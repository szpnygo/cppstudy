#pragma once
#include "base.h"
#include "hash_store.h"

#include <cstddef>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

constexpr size_t kShardCount = 128;

class ShardedHashStore : public BaseStore {
  public:
    ShardedHashStore(const std::string& filename)
        : filename_(filename),
          mutexes_(kShardCount),
          kv_stores_(kShardCount),
          count_(kShardCount) {}

    std::string Get(const std::string& key) override;
    void Set(const std::string& key, const std::string& value) override;
    void Load() override;
    void Save() override;

  private:
    size_t count_;
    // 用于分片的互斥锁
    std::vector<std::mutex> mutexes_;
    // 分片的kv存储
    std::vector<std::unordered_map<std::string, std::string>> kv_stores_;
    // 文件名
    std::string filename_;
};