/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Distributed under BSD 3-Clause license.                                   *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Illinois Institute of Technology.                        *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of Hermes. The full Hermes copyright notice, including  *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the top directory. If you do not  *
 * have access to the file, you may request a copy from help@hdfgroup.org.   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool mpiio_intercepted = true;

#include <hermes.h>
#include <bucket.h>

#include "mpiio_api.h"
#include "mpiio_fs_api.h"

#include "constants.h"
#include "hermes_shm/util/singleton.h"
#include "interceptor.h"

/**
 * Namespace declarations
 */
using hermes::ThreadPool;
using hermes::adapter::fs::MetadataManager;
using hermes::adapter::fs::File;
using hermes::adapter::fs::AdapterStat;
using hermes::adapter::fs::MpiioApi;
using hermes::adapter::fs::MpiioFs;
using hermes::adapter::fs::MpiioSeekModeConv;

namespace hapi = hermes::api;

extern "C" {

/**
 * MPI
 */
int HERMES_DECL(MPI_Init)(int *argc, char ***argv) {
  LOG(INFO) << "MPI Init intercepted." << std::endl;
  TRANSPARENT_HERMES
  auto real_api = HERMES_MPIIO_API;
  return real_api->MPI_Init(argc, argv);
}

int HERMES_DECL(MPI_Finalize)(void) {
  LOG(INFO) << "MPI Finalize intercepted." << std::endl;
  auto real_api = HERMES_MPIIO_API;
  return real_api->MPI_Finalize();
}

int HERMES_DECL(MPI_Wait)(MPI_Request *req, MPI_Status *status) {
  LOG(INFO) << "In MPI_Wait." << std::endl;
  auto fs_api = HERMES_MPIIO_FS;
  return fs_api->Wait(req, status);
}

int HERMES_DECL(MPI_Waitall)(int count, MPI_Request *req, MPI_Status *status) {
  LOG(INFO) << "In MPI_Waitall." << std::endl;
  auto fs_api = HERMES_MPIIO_FS;
  return fs_api->WaitAll(count, req, status);
}

/**
 * Metadata functions
 */
int HERMES_DECL(MPI_File_open)(MPI_Comm comm, const char *filename, int amode,
                               MPI_Info info, MPI_File *fh) {
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "Intercept MPI_File_open for filename: " << filename
            << " and mode: " << amode << std::endl;
  if (fs_api->IsPathTracked(filename)) {
    LOG(INFO) << "Intercept MPI_File_open: " << std::endl;
    AdapterStat stat;
    stat.comm_ = comm;
    stat.amode_ = amode;
    stat.info_ = info;
    File f = fs_api->Open(stat, filename);
    (*fh) = stat.mpi_fh_;
    return f.mpi_status_;
  }
  return real_api->MPI_File_open(comm, filename, amode, info, fh);
}

int HERMES_DECL(MPI_File_close)(MPI_File *fh) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_seek." << std::endl;
  if (fs_api->IsMpiFpTracked(fh)) {
    LOG(INFO) << "Intercept MPI_File_seek." << std::endl;
    File f; f.hermes_mpi_fh_ = *fh;
    return fs_api->Close(f, stat_exists);
  }
  return real_api->MPI_File_close(fh);
}

int HERMES_DECL(MPI_File_seek)(MPI_File fh, MPI_Offset offset, int whence) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_seek." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_seek." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->Seek(f, stat_exists, offset, whence);
  }
  return real_api->MPI_File_seek(fh, offset, whence);
}

int HERMES_DECL(MPI_File_seek_shared)(MPI_File fh, MPI_Offset offset,
                                      int whence) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_seek_shared." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_seek_shared offset:" << offset
              << " whence:" << whence << "." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->SeekShared(f, stat_exists, offset, whence);
  }
  return real_api->MPI_File_seek_shared(fh, offset, whence);
}

int HERMES_DECL(MPI_File_get_position)(MPI_File fh, MPI_Offset *offset) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_get_position." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_get_position." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    (*offset) = static_cast<MPI_Offset>(fs_api->Tell(f, stat_exists));
    return MPI_SUCCESS;
  }
  return real_api->MPI_File_get_position(fh, offset);
}

int HERMES_DECL(MPI_File_read_all)(MPI_File fh, void *buf, int count,
                                   MPI_Datatype datatype, MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_read_all." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_read_all." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->ReadAll(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_read_all(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_read_at_all)(MPI_File fh, MPI_Offset offset, void *buf,
                                      int count, MPI_Datatype datatype,
                                      MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_read_at_all." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_read_at_all." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->ReadAll(f, stat_exists, buf, offset, count, datatype,
                           status);
  }
  return real_api->MPI_File_read_at_all(fh, offset, buf, count, datatype,
                                        status);
}
int HERMES_DECL(MPI_File_read_at)(MPI_File fh, MPI_Offset offset, void *buf,
                                  int count, MPI_Datatype datatype,
                                  MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_read_at." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_read_at." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->Read(f, stat_exists, buf, offset, count, datatype, status);
  }
  return real_api->MPI_File_read_at(fh, offset, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_read)(MPI_File fh, void *buf, int count,
                               MPI_Datatype datatype, MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_read." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_read." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    int ret = fs_api->Read(f, stat_exists, buf, count, datatype, status);
    if (stat_exists) return ret;
  }
  return real_api->MPI_File_read(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_read_ordered)(MPI_File fh, void *buf, int count,
                                       MPI_Datatype datatype,
                                       MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_read_ordered." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_read_ordered." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->ReadOrdered(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_read_ordered(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_read_shared)(MPI_File fh, void *buf, int count,
                                      MPI_Datatype datatype,
                                      MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_read_shared." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_read_shared." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->Read(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_read_shared(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_write_all)(MPI_File fh, const void *buf, int count,
                                    MPI_Datatype datatype, MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_write_all." << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_write_all." << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->WriteAll(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_write_all(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_write_at_all)(MPI_File fh, MPI_Offset offset,
                                       const void *buf, int count,
                                       MPI_Datatype datatype,
                                       MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_write_at_all" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_write_at_all" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->WriteAll(f, stat_exists, buf,
                            offset, count, datatype, status);
  }
  return real_api->MPI_File_write_at_all(fh, offset, buf, count, datatype,
                                         status);
}
int HERMES_DECL(MPI_File_write_at)(MPI_File fh, MPI_Offset offset,
                                   const void *buf, int count,
                                   MPI_Datatype datatype, MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_write_at" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_write_at" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->Write(f, stat_exists, buf, offset, count, datatype, status);
  }
  return real_api->MPI_File_write_at(fh, offset, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_write)(MPI_File fh, const void *buf, int count,
                                MPI_Datatype datatype, MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_write" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_write" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->Write(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_write(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_write_ordered)(MPI_File fh, const void *buf, int count,
                                        MPI_Datatype datatype,
                                        MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_write_shared" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_write_ordered" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->WriteOrdered(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_write_ordered(fh, buf, count, datatype, status);
}
int HERMES_DECL(MPI_File_write_shared)(MPI_File fh, const void *buf, int count,
                                       MPI_Datatype datatype,
                                       MPI_Status *status) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_write_shared" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    // NOTE(llogan): originally WriteOrdered
    LOG(INFO) << "Intercept MPI_File_write_shared" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    return fs_api->WriteOrdered(f, stat_exists, buf, count, datatype, status);
  }
  return real_api->MPI_File_write_shared(fh, buf, count, datatype, status);
}

/**
 * Async Read/Write
 */
int HERMES_DECL(MPI_File_iread_at)(MPI_File fh, MPI_Offset offset, void *buf,
                                   int count, MPI_Datatype datatype,
                                   MPI_Request *request) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iread_at" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_iread_at" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->ARead(f, stat_exists, buf, offset, count, datatype, request);
    return MPI_SUCCESS;
  }
  return real_api->MPI_File_iread_at(fh, offset, buf, count, datatype, request);
}
int HERMES_DECL(MPI_File_iread)(MPI_File fh, void *buf, int count,
                                MPI_Datatype datatype, MPI_Request *request) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iread" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_iread" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->ARead(f, stat_exists, buf, count, datatype, request);
  }
  return real_api->MPI_File_iread(fh, buf, count, datatype, request);
}
int HERMES_DECL(MPI_File_iread_shared)(MPI_File fh, void *buf, int count,
                                       MPI_Datatype datatype,
                                       MPI_Request *request) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iread_shared" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_iread_shared" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->ARead(f, stat_exists, buf, count, datatype, request);
    return MPI_SUCCESS;
  }
  return real_api->MPI_File_iread_shared(fh, buf, count, datatype, request);
}
int HERMES_DECL(MPI_File_iwrite_at)(MPI_File fh, MPI_Offset offset,
                                    const void *buf, int count,
                                    MPI_Datatype datatype,
                                    MPI_Request *request) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iwrite_at" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_iwrite_at" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->AWrite(f, stat_exists, buf, offset, count, datatype, request);
    return MPI_SUCCESS;
  }
  return real_api->MPI_File_iwrite_at(fh, offset, buf, count, datatype,
                                      request);
}

int HERMES_DECL(MPI_File_iwrite)(MPI_File fh, const void *buf, int count,
                                 MPI_Datatype datatype, MPI_Request *request) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iwrite" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_iwrite" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->AWrite(f, stat_exists, buf, count, datatype, request);
    return MPI_SUCCESS;
  }
  return real_api->MPI_File_iwrite(fh, buf, count, datatype, request);
}
int HERMES_DECL(MPI_File_iwrite_shared)(MPI_File fh, const void *buf, int count,
                                        MPI_Datatype datatype,
                                        MPI_Request *request) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iwrite_shared" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_iwrite_shared" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->AWriteOrdered(f, stat_exists, buf, count, datatype, request);
    return MPI_SUCCESS;
  }
  return real_api->MPI_File_iwrite_shared(fh, buf, count, datatype, request);
}

/**
 * Other functions
 */
int HERMES_DECL(MPI_File_sync)(MPI_File fh) {
  bool stat_exists;
  auto real_api = HERMES_MPIIO_API;
  auto fs_api = HERMES_MPIIO_FS;
  LOG(INFO) << "In MPI_File_iwrite_shared" << std::endl;
  if (fs_api->IsMpiFpTracked(&fh)) {
    LOG(INFO) << "Intercept MPI_File_sync" << std::endl;
    File f; f.hermes_mpi_fh_ = fh;
    fs_api->Sync(f, stat_exists);
    return 0;
  }
  return real_api->MPI_File_sync(fh);
}

}  // extern C
