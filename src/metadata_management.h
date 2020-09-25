#ifndef HERMES_METADATA_MANAGEMENT_H_
#define HERMES_METADATA_MANAGEMENT_H_

#include <string.h>

#include <atomic>
#include <string>

#include "memory_management.h"
#include "buffer_pool.h"

namespace hermes {

struct RpcContext;

enum MapType {
  kMapType_Bucket,
  kMapType_VBucket,
  kMapType_Blob,
};

union VBucketID {
  struct {
    u32 index;
    u32 node_id;
  } bits;

  u64 as_int;
};

union BlobID {
  struct {
    u32 buffer_ids_offset;
    i32 node_id;
  } bits;

  u64 as_int;
};

enum class TraitID : u8 {
  None,
  Placement,
  Flush,
  Replication,
  Checksum,
  BlobSort,
  BlobFilter,
  Index,

  Count,
};

struct Stats {
};

const int kIdListChunkSize = 10;

struct ChunkedIdList {
  u32 head_offset;
  u32 length;
  u32 capacity;
};

struct IdList {
  u32 head_offset;
  u32 length;
};

struct BufferIdArray {
  BufferID *ids;
  u32 length;
};

struct BucketInfo {
  BucketID next_free;
  ChunkedIdList blobs;
  std::atomic<int> ref_count;
  bool active;
  Stats stats;
};

static constexpr int kMaxTraitsPerVBucket = 8;

struct VBucketInfo {
  VBucketID next_free;
  ChunkedIdList blobs;
  std::atomic<int> ref_count;
  TraitID traits[kMaxTraitsPerVBucket];
  bool active;
  Stats stats;
};

struct SystemViewState {
  std::atomic<u64> bytes_available[kMaxDevices];
  int num_devices;
};

struct MetadataManager {
  // All offsets are relative to the beginning of the MDM
  ptrdiff_t bucket_info_offset;
  BucketID first_free_bucket;

  ptrdiff_t vbucket_info_offset;
  VBucketID first_free_vbucket;

  ptrdiff_t rpc_state_offset;
  ptrdiff_t system_view_state_offset;
  ptrdiff_t global_system_view_state_offset;

  ptrdiff_t id_heap_offset;
  ptrdiff_t map_heap_offset;

  ptrdiff_t bucket_map_offset;
  ptrdiff_t vbucket_map_offset;
  ptrdiff_t blob_map_offset;

  ptrdiff_t swap_filename_prefix_offset;
  ptrdiff_t swap_filename_suffix_offset;

  TicketMutex bucket_mutex;
  TicketMutex vbucket_mutex;

  TicketMutex bucket_map_mutex;
  TicketMutex vbucket_map_mutex;
  TicketMutex blob_map_mutex;
  TicketMutex id_mutex;

  size_t map_seed;

  IdList node_targets;

  u32 system_view_state_update_interval_ms;
  u32 global_system_view_state_node_id;
  u32 num_buckets;
  u32 max_buckets;
  u32 num_vbuckets;
  u32 max_vbuckets;
};

struct RpcContext;

/**
 *
 */
void InitMetadataManager(MetadataManager *mdm, Arena *arena, Config *config,
                         int node_id);

/**
 *
 */
void DestroyBucket(SharedMemoryContext *context, RpcContext *rpc,
                   const char *name, BucketID bucket_id);

/**
 *
 */
void DestroyBlobByName(SharedMemoryContext *context, RpcContext *rpc,
                       BucketID bucket_id, const std::string &blob_name);

/**
 *
 */
void RenameBlob(SharedMemoryContext *context, RpcContext *rpc,
                const std::string &old_name, const std::string &new_name);

/**
 *
 */
void RenameBucket(SharedMemoryContext *context, RpcContext *rpc, BucketID id,
                  const std::string &old_name, const std::string &new_name);

/**
 *
 */
bool ContainsBlob(SharedMemoryContext *context, RpcContext *rpc,
                  BucketID bucket_id, const std::string &blob_name);

/**
 *
 */
BufferIdArray GetBufferIdsFromBlobName(Arena *arena,
                                       SharedMemoryContext *context,
                                       RpcContext *rpc, const char *blob_name,
                                       u32 **sizes);

/**
 *
 */
BucketID GetOrCreateBucketId(SharedMemoryContext *context, RpcContext *rpc,
                             const std::string &name);

/**
 *
 */
void AttachBlobToBucket(SharedMemoryContext *context, RpcContext *rpc,
                        const char *blob_name, BucketID bucket_id,
                        const std::vector<BufferID> &buffer_ids,
                        bool is_swap_blob = false);

/**
 *
 */
void IncrementRefcount(SharedMemoryContext *context, RpcContext *rpc,
                       BucketID id);

/**
 *
 */
void DecrementRefcount(SharedMemoryContext *context, RpcContext *rpc,
                       BucketID id);

/**
 *
 */
std::vector<BufferID> SwapBlobToVec(SwapBlob swap_blob);

/**
 *
 */
SwapBlob VecToSwapBlob(std::vector<BufferID> &vec);

// internal
u32 HashString(MetadataManager *mdm, RpcContext *rpc, const char *str);
MetadataManager *GetMetadataManagerFromContext(SharedMemoryContext *context);
BucketInfo *LocalGetBucketInfoByIndex(MetadataManager *mdm, u32 index);
VBucketInfo *GetVBucketInfoByIndex(MetadataManager *mdm, u32 index);

void LocalAddBlobIdToBucket(MetadataManager *mdm, BucketID bucket_id,
                            BlobID blob_id);
std::vector<BufferID> LocalGetBufferIdList(MetadataManager *mdm,
                                           BlobID blob_id);
void LocalGetBufferIdList(Arena *arena, MetadataManager *mdm, BlobID blob_id,
                          BufferIdArray *buffer_ids);
void LocalFreeBufferIdList(SharedMemoryContext *context, BlobID blob_id);
void LocalDestroyBucket(SharedMemoryContext *context, RpcContext *rpc,
                        const char *bucket_name, BucketID bucket_id);
void LocalDestroyBlobById(SharedMemoryContext *context, RpcContext *rpc,
                          BlobID blob_id);
void LocalDestroyBlobByName(SharedMemoryContext *context, RpcContext *rpc,
                            const char *blob_name, BlobID blob_id);
BucketID LocalGetNextFreeBucketId(SharedMemoryContext *context, RpcContext *rpc,
                                  const std::string &name);
u32 LocalAllocateBufferIdList(MetadataManager *mdm,
                              const std::vector<BufferID> &buffer_ids);
void LocalRenameBucket(SharedMemoryContext *context, RpcContext *rpc,
                       BucketID id, const std::string &old_name,
                       const std::string &new_name);
bool LocalContainsBlob(SharedMemoryContext *context, BucketID bucket_id,
                       BlobID blob_id);
void LocalRemoveBlobFromBucketInfo(SharedMemoryContext *context,
                                   BucketID bucket_id, BlobID blob_id);
void LocalIncrementRefcount(SharedMemoryContext *context, BucketID id);
void LocalDecrementRefcount(SharedMemoryContext *context, BucketID id);

u64 LocalGet(MetadataManager *mdm, const char *key, MapType map_type);
void LocalPut(MetadataManager *mdm, const char *key, u64 val, MapType map_type);
void LocalDelete(MetadataManager *mdm, const char *key, MapType map_type);

u64 LocalGetRemainingCapacity(SharedMemoryContext *context, TargetID id);
void LocalUpdateGlobalSystemViewState(SharedMemoryContext *context,
                                      std::vector<i64> adjustments);
SystemViewState *GetLocalSystemViewState(SharedMemoryContext *context);
SystemViewState *GetGlobalSystemViewState(SharedMemoryContext *context);
std::vector<u64> LocalGetGlobalDeviceCapacities(SharedMemoryContext *context);
std::vector<u64> GetGlobalDeviceCapacities(SharedMemoryContext *context,
                                           RpcContext *rpc);
void UpdateGlobalSystemViewState(SharedMemoryContext *context,
                                 RpcContext *rpc);

void StartGlobalSystemViewStateUpdateThread(SharedMemoryContext *context,
                                            RpcContext *rpc, Arena *arena,
                                            double slepp_ms);

void InitMetadataStorage(SharedMemoryContext *context, MetadataManager *mdm,
                         Arena *arena, Config *config, i32 node_id);

std::vector<u64> GetRemainingNodeCapacities(SharedMemoryContext *context);
std::string GetSwapFilename(MetadataManager *mdm, u32 node_id);

}  // namespace hermes

#endif  // HERMES_METADATA_MANAGEMENT_H_
