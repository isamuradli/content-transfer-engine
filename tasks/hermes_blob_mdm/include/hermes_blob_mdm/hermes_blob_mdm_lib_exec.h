#ifndef LABSTOR_HERMES_BLOB_MDM_LIB_EXEC_H_
#define LABSTOR_HERMES_BLOB_MDM_LIB_EXEC_H_

/** Execute a task */
void Run(u32 method, Task *task) override {
  switch (method) {
    case Method::kConstruct: {
      Construct(reinterpret_cast<ConstructTask *>(task));
      break;
    }
    case Method::kDestruct: {
      Destruct(reinterpret_cast<DestructTask *>(task));
      break;
    }
    case Method::kPutBlob: {
      PutBlob(reinterpret_cast<PutBlobTask *>(task));
      break;
    }
    case Method::kGetBlob: {
      GetBlob(reinterpret_cast<GetBlobTask *>(task));
      break;
    }
    case Method::kTruncateBlob: {
      TruncateBlob(reinterpret_cast<TruncateBlobTask *>(task));
      break;
    }
    case Method::kDestroyBlob: {
      DestroyBlob(reinterpret_cast<DestroyBlobTask *>(task));
      break;
    }
    case Method::kTagBlob: {
      TagBlob(reinterpret_cast<TagBlobTask *>(task));
      break;
    }
    case Method::kBlobHasTag: {
      BlobHasTag(reinterpret_cast<BlobHasTagTask *>(task));
      break;
    }
    case Method::kGetBlobId: {
      GetBlobId(reinterpret_cast<GetBlobIdTask *>(task));
      break;
    }
    case Method::kGetOrCreateBlobId: {
      GetOrCreateBlobId(reinterpret_cast<GetOrCreateBlobIdTask *>(task));
      break;
    }
    case Method::kGetBlobName: {
      GetBlobName(reinterpret_cast<GetBlobNameTask *>(task));
      break;
    }
    case Method::kGetBlobSize: {
      GetBlobSize(reinterpret_cast<GetBlobSizeTask *>(task));
      break;
    }
    case Method::kGetBlobScore: {
      GetBlobScore(reinterpret_cast<GetBlobScoreTask *>(task));
      break;
    }
    case Method::kGetBlobBuffers: {
      GetBlobBuffers(reinterpret_cast<GetBlobBuffersTask *>(task));
      break;
    }
    case Method::kRenameBlob: {
      RenameBlob(reinterpret_cast<RenameBlobTask *>(task));
      break;
    }
    case Method::kReorganizeBlob: {
      ReorganizeBlob(reinterpret_cast<ReorganizeBlobTask *>(task));
      break;
    }
    case Method::kSetBucketMdm: {
      SetBucketMdm(reinterpret_cast<SetBucketMdmTask *>(task));
      break;
    }
  }
}
/** Ensure there is space to store replicated outputs */
void ReplicateStart(u32 method, u32 count, Task *task) override {
  switch (method) {
    case Method::kConstruct: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<ConstructTask*>(task));
      break;
    }
    case Method::kDestruct: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<DestructTask*>(task));
      break;
    }
    case Method::kPutBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<PutBlobTask*>(task));
      break;
    }
    case Method::kGetBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetBlobTask*>(task));
      break;
    }
    case Method::kTruncateBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<TruncateBlobTask*>(task));
      break;
    }
    case Method::kDestroyBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<DestroyBlobTask*>(task));
      break;
    }
    case Method::kTagBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<TagBlobTask*>(task));
      break;
    }
    case Method::kBlobHasTag: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<BlobHasTagTask*>(task));
      break;
    }
    case Method::kGetBlobId: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetBlobIdTask*>(task));
      break;
    }
    case Method::kGetOrCreateBlobId: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetOrCreateBlobIdTask*>(task));
      break;
    }
    case Method::kGetBlobName: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetBlobNameTask*>(task));
      break;
    }
    case Method::kGetBlobSize: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetBlobSizeTask*>(task));
      break;
    }
    case Method::kGetBlobScore: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetBlobScoreTask*>(task));
      break;
    }
    case Method::kGetBlobBuffers: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<GetBlobBuffersTask*>(task));
      break;
    }
    case Method::kRenameBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<RenameBlobTask*>(task));
      break;
    }
    case Method::kReorganizeBlob: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<ReorganizeBlobTask*>(task));
      break;
    }
    case Method::kSetBucketMdm: {
      labstor::CALL_REPLICA_START(count, reinterpret_cast<SetBucketMdmTask*>(task));
      break;
    }
  }
}
/** Determine success and handle failures */
void ReplicateEnd(u32 method, Task *task) override {
  switch (method) {
    case Method::kConstruct: {
      labstor::CALL_REPLICA_END(reinterpret_cast<ConstructTask*>(task));
      break;
    }
    case Method::kDestruct: {
      labstor::CALL_REPLICA_END(reinterpret_cast<DestructTask*>(task));
      break;
    }
    case Method::kPutBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<PutBlobTask*>(task));
      break;
    }
    case Method::kGetBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetBlobTask*>(task));
      break;
    }
    case Method::kTruncateBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<TruncateBlobTask*>(task));
      break;
    }
    case Method::kDestroyBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<DestroyBlobTask*>(task));
      break;
    }
    case Method::kTagBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<TagBlobTask*>(task));
      break;
    }
    case Method::kBlobHasTag: {
      labstor::CALL_REPLICA_END(reinterpret_cast<BlobHasTagTask*>(task));
      break;
    }
    case Method::kGetBlobId: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetBlobIdTask*>(task));
      break;
    }
    case Method::kGetOrCreateBlobId: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetOrCreateBlobIdTask*>(task));
      break;
    }
    case Method::kGetBlobName: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetBlobNameTask*>(task));
      break;
    }
    case Method::kGetBlobSize: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetBlobSizeTask*>(task));
      break;
    }
    case Method::kGetBlobScore: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetBlobScoreTask*>(task));
      break;
    }
    case Method::kGetBlobBuffers: {
      labstor::CALL_REPLICA_END(reinterpret_cast<GetBlobBuffersTask*>(task));
      break;
    }
    case Method::kRenameBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<RenameBlobTask*>(task));
      break;
    }
    case Method::kReorganizeBlob: {
      labstor::CALL_REPLICA_END(reinterpret_cast<ReorganizeBlobTask*>(task));
      break;
    }
    case Method::kSetBucketMdm: {
      labstor::CALL_REPLICA_END(reinterpret_cast<SetBucketMdmTask*>(task));
      break;
    }
  }
}
/** Serialize a task when initially pushing into remote */
std::vector<DataTransfer> SaveStart(u32 method, BinaryOutputArchive<true> &ar, Task *task) override {
  switch (method) {
    case Method::kConstruct: {
      ar << *reinterpret_cast<ConstructTask*>(task);
      break;
    }
    case Method::kDestruct: {
      ar << *reinterpret_cast<DestructTask*>(task);
      break;
    }
    case Method::kPutBlob: {
      ar << *reinterpret_cast<PutBlobTask*>(task);
      break;
    }
    case Method::kGetBlob: {
      ar << *reinterpret_cast<GetBlobTask*>(task);
      break;
    }
    case Method::kTruncateBlob: {
      ar << *reinterpret_cast<TruncateBlobTask*>(task);
      break;
    }
    case Method::kDestroyBlob: {
      ar << *reinterpret_cast<DestroyBlobTask*>(task);
      break;
    }
    case Method::kTagBlob: {
      ar << *reinterpret_cast<TagBlobTask*>(task);
      break;
    }
    case Method::kBlobHasTag: {
      ar << *reinterpret_cast<BlobHasTagTask*>(task);
      break;
    }
    case Method::kGetBlobId: {
      ar << *reinterpret_cast<GetBlobIdTask*>(task);
      break;
    }
    case Method::kGetOrCreateBlobId: {
      ar << *reinterpret_cast<GetOrCreateBlobIdTask*>(task);
      break;
    }
    case Method::kGetBlobName: {
      ar << *reinterpret_cast<GetBlobNameTask*>(task);
      break;
    }
    case Method::kGetBlobSize: {
      ar << *reinterpret_cast<GetBlobSizeTask*>(task);
      break;
    }
    case Method::kGetBlobScore: {
      ar << *reinterpret_cast<GetBlobScoreTask*>(task);
      break;
    }
    case Method::kGetBlobBuffers: {
      ar << *reinterpret_cast<GetBlobBuffersTask*>(task);
      break;
    }
    case Method::kRenameBlob: {
      ar << *reinterpret_cast<RenameBlobTask*>(task);
      break;
    }
    case Method::kReorganizeBlob: {
      ar << *reinterpret_cast<ReorganizeBlobTask*>(task);
      break;
    }
    case Method::kSetBucketMdm: {
      ar << *reinterpret_cast<SetBucketMdmTask*>(task);
      break;
    }
  }
  return ar.Get();
}
/** Deserialize a task when popping from remote queue */
TaskPointer LoadStart(u32 method, BinaryInputArchive<true> &ar) override {
  TaskPointer task_ptr;
  switch (method) {
    case Method::kConstruct: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<ConstructTask>(task_ptr.p_);
      ar >> *reinterpret_cast<ConstructTask*>(task_ptr.task_);
      break;
    }
    case Method::kDestruct: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<DestructTask>(task_ptr.p_);
      ar >> *reinterpret_cast<DestructTask*>(task_ptr.task_);
      break;
    }
    case Method::kPutBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<PutBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<PutBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kTruncateBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<TruncateBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<TruncateBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kDestroyBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<DestroyBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<DestroyBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kTagBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<TagBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<TagBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kBlobHasTag: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<BlobHasTagTask>(task_ptr.p_);
      ar >> *reinterpret_cast<BlobHasTagTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetBlobId: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetBlobIdTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetBlobIdTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetOrCreateBlobId: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetOrCreateBlobIdTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetOrCreateBlobIdTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetBlobName: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetBlobNameTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetBlobNameTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetBlobSize: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetBlobSizeTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetBlobSizeTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetBlobScore: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetBlobScoreTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetBlobScoreTask*>(task_ptr.task_);
      break;
    }
    case Method::kGetBlobBuffers: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<GetBlobBuffersTask>(task_ptr.p_);
      ar >> *reinterpret_cast<GetBlobBuffersTask*>(task_ptr.task_);
      break;
    }
    case Method::kRenameBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<RenameBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<RenameBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kReorganizeBlob: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<ReorganizeBlobTask>(task_ptr.p_);
      ar >> *reinterpret_cast<ReorganizeBlobTask*>(task_ptr.task_);
      break;
    }
    case Method::kSetBucketMdm: {
      task_ptr.task_ = LABSTOR_CLIENT->NewEmptyTask<SetBucketMdmTask>(task_ptr.p_);
      ar >> *reinterpret_cast<SetBucketMdmTask*>(task_ptr.task_);
      break;
    }
  }
  return task_ptr;
}
/** Serialize a task when returning from remote queue */
std::vector<DataTransfer> SaveEnd(u32 method, BinaryOutputArchive<false> &ar, Task *task) override {
  switch (method) {
    case Method::kConstruct: {
      ar << *reinterpret_cast<ConstructTask*>(task);
      break;
    }
    case Method::kDestruct: {
      ar << *reinterpret_cast<DestructTask*>(task);
      break;
    }
    case Method::kPutBlob: {
      ar << *reinterpret_cast<PutBlobTask*>(task);
      break;
    }
    case Method::kGetBlob: {
      ar << *reinterpret_cast<GetBlobTask*>(task);
      break;
    }
    case Method::kTruncateBlob: {
      ar << *reinterpret_cast<TruncateBlobTask*>(task);
      break;
    }
    case Method::kDestroyBlob: {
      ar << *reinterpret_cast<DestroyBlobTask*>(task);
      break;
    }
    case Method::kTagBlob: {
      ar << *reinterpret_cast<TagBlobTask*>(task);
      break;
    }
    case Method::kBlobHasTag: {
      ar << *reinterpret_cast<BlobHasTagTask*>(task);
      break;
    }
    case Method::kGetBlobId: {
      ar << *reinterpret_cast<GetBlobIdTask*>(task);
      break;
    }
    case Method::kGetOrCreateBlobId: {
      ar << *reinterpret_cast<GetOrCreateBlobIdTask*>(task);
      break;
    }
    case Method::kGetBlobName: {
      ar << *reinterpret_cast<GetBlobNameTask*>(task);
      break;
    }
    case Method::kGetBlobSize: {
      ar << *reinterpret_cast<GetBlobSizeTask*>(task);
      break;
    }
    case Method::kGetBlobScore: {
      ar << *reinterpret_cast<GetBlobScoreTask*>(task);
      break;
    }
    case Method::kGetBlobBuffers: {
      ar << *reinterpret_cast<GetBlobBuffersTask*>(task);
      break;
    }
    case Method::kRenameBlob: {
      ar << *reinterpret_cast<RenameBlobTask*>(task);
      break;
    }
    case Method::kReorganizeBlob: {
      ar << *reinterpret_cast<ReorganizeBlobTask*>(task);
      break;
    }
    case Method::kSetBucketMdm: {
      ar << *reinterpret_cast<SetBucketMdmTask*>(task);
      break;
    }
  }
  return ar.Get();
}
/** Deserialize a task when returning from remote queue */
void LoadEnd(u32 replica, u32 method, BinaryInputArchive<false> &ar, Task *task) override {
  switch (method) {
    case Method::kConstruct: {
      ar.Deserialize(replica, *reinterpret_cast<ConstructTask*>(task));
      break;
    }
    case Method::kDestruct: {
      ar.Deserialize(replica, *reinterpret_cast<DestructTask*>(task));
      break;
    }
    case Method::kPutBlob: {
      ar.Deserialize(replica, *reinterpret_cast<PutBlobTask*>(task));
      break;
    }
    case Method::kGetBlob: {
      ar.Deserialize(replica, *reinterpret_cast<GetBlobTask*>(task));
      break;
    }
    case Method::kTruncateBlob: {
      ar.Deserialize(replica, *reinterpret_cast<TruncateBlobTask*>(task));
      break;
    }
    case Method::kDestroyBlob: {
      ar.Deserialize(replica, *reinterpret_cast<DestroyBlobTask*>(task));
      break;
    }
    case Method::kTagBlob: {
      ar.Deserialize(replica, *reinterpret_cast<TagBlobTask*>(task));
      break;
    }
    case Method::kBlobHasTag: {
      ar.Deserialize(replica, *reinterpret_cast<BlobHasTagTask*>(task));
      break;
    }
    case Method::kGetBlobId: {
      ar.Deserialize(replica, *reinterpret_cast<GetBlobIdTask*>(task));
      break;
    }
    case Method::kGetOrCreateBlobId: {
      ar.Deserialize(replica, *reinterpret_cast<GetOrCreateBlobIdTask*>(task));
      break;
    }
    case Method::kGetBlobName: {
      ar.Deserialize(replica, *reinterpret_cast<GetBlobNameTask*>(task));
      break;
    }
    case Method::kGetBlobSize: {
      ar.Deserialize(replica, *reinterpret_cast<GetBlobSizeTask*>(task));
      break;
    }
    case Method::kGetBlobScore: {
      ar.Deserialize(replica, *reinterpret_cast<GetBlobScoreTask*>(task));
      break;
    }
    case Method::kGetBlobBuffers: {
      ar.Deserialize(replica, *reinterpret_cast<GetBlobBuffersTask*>(task));
      break;
    }
    case Method::kRenameBlob: {
      ar.Deserialize(replica, *reinterpret_cast<RenameBlobTask*>(task));
      break;
    }
    case Method::kReorganizeBlob: {
      ar.Deserialize(replica, *reinterpret_cast<ReorganizeBlobTask*>(task));
      break;
    }
    case Method::kSetBucketMdm: {
      ar.Deserialize(replica, *reinterpret_cast<SetBucketMdmTask*>(task));
      break;
    }
  }
}
/** Get the grouping of the task */
u32 GetGroup(u32 method, Task *task, hshm::charbuf &group) override {
  switch (method) {
    case Method::kConstruct: {
      return reinterpret_cast<ConstructTask*>(task)->GetGroup(group);
    }
    case Method::kDestruct: {
      return reinterpret_cast<DestructTask*>(task)->GetGroup(group);
    }
    case Method::kPutBlob: {
      return reinterpret_cast<PutBlobTask*>(task)->GetGroup(group);
    }
    case Method::kGetBlob: {
      return reinterpret_cast<GetBlobTask*>(task)->GetGroup(group);
    }
    case Method::kTruncateBlob: {
      return reinterpret_cast<TruncateBlobTask*>(task)->GetGroup(group);
    }
    case Method::kDestroyBlob: {
      return reinterpret_cast<DestroyBlobTask*>(task)->GetGroup(group);
    }
    case Method::kTagBlob: {
      return reinterpret_cast<TagBlobTask*>(task)->GetGroup(group);
    }
    case Method::kBlobHasTag: {
      return reinterpret_cast<BlobHasTagTask*>(task)->GetGroup(group);
    }
    case Method::kGetBlobId: {
      return reinterpret_cast<GetBlobIdTask*>(task)->GetGroup(group);
    }
    case Method::kGetOrCreateBlobId: {
      return reinterpret_cast<GetOrCreateBlobIdTask*>(task)->GetGroup(group);
    }
    case Method::kGetBlobName: {
      return reinterpret_cast<GetBlobNameTask*>(task)->GetGroup(group);
    }
    case Method::kGetBlobSize: {
      return reinterpret_cast<GetBlobSizeTask*>(task)->GetGroup(group);
    }
    case Method::kGetBlobScore: {
      return reinterpret_cast<GetBlobScoreTask*>(task)->GetGroup(group);
    }
    case Method::kGetBlobBuffers: {
      return reinterpret_cast<GetBlobBuffersTask*>(task)->GetGroup(group);
    }
    case Method::kRenameBlob: {
      return reinterpret_cast<RenameBlobTask*>(task)->GetGroup(group);
    }
    case Method::kReorganizeBlob: {
      return reinterpret_cast<ReorganizeBlobTask*>(task)->GetGroup(group);
    }
    case Method::kSetBucketMdm: {
      return reinterpret_cast<SetBucketMdmTask*>(task)->GetGroup(group);
    }
  }
  return -1;
}

#endif  // LABSTOR_HERMES_BLOB_MDM_METHODS_H_