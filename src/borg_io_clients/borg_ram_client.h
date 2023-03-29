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

#ifndef HERMES_SRC_BORG_IO_CLIENTS_RAM_H_
#define HERMES_SRC_BORG_IO_CLIENTS_RAM_H_

#include "borg_io_client.h"
#include "io_client/posix/posix_api.h"
#include "hermes.h"

namespace hermes::borg {

class RamIoClient : public BorgIoClient {
 public:
  virtual ~RamIoClient() = default;

  bool Init(DeviceInfo &dev_info) override {
    auto &hermes_header = HERMES->header_;
    auto &main_alloc = HERMES->main_alloc_;
    hermes_header->ram_tier_ = main_alloc->
                               Allocate(dev_info.header_->capacity_);
    if (hermes_header->ram_tier_.IsNull()) {
      LOG(FATAL) << BUFFER_POOL_OUT_OF_RAM.Msg() << std::endl;
    }
    return true;
  }

  bool Write(DeviceInfo &dev_info, const char *data,
             size_t off, size_t size) override {
    auto &hermes_header = HERMES->header_;
    auto &main_alloc = HERMES->main_alloc_;
    char *ram_ptr = main_alloc->Convert<char>(hermes_header->ram_tier_);
    if (off + size > dev_info.header_->capacity_) {
      LOG(INFO) << hshm::Formatter::format(
        "Out of bounds: attempting to write to offset: {} / {}",
                       off + size,
                       dev_info.header_->capacity_) << std::endl;
      return false;
    }
    memcpy(ram_ptr + off, data, size);
    return true;
  }

  bool Read(DeviceInfo &dev_info, char *data,
            size_t off, size_t size) override {
    auto &hermes_header = HERMES->header_;
    auto &main_alloc = HERMES->main_alloc_;
    char *ram_ptr = main_alloc->Convert<char>(hermes_header->ram_tier_);
    memcpy(data, ram_ptr + off, size);
    return true;
  }
};

}  // namespace hermes::borg

#endif  // HERMES_SRC_BORG_IO_CLIENTS_RAM_H_
