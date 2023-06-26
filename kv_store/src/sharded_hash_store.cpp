#include "sharded_hash_store.h"

#include <fstream>
#include <mutex>

void ShardedHashStore::Load() {
    std::ifstream file(filename_);
    if (file.fail()) {
        throw std::runtime_error("Failed to open file for reading");
    }
    std::string key;
    std::string value;

    // 从文件中读取键值对，直到文件结束
    while (file >> key >> value) {
        size_t lock_index = std::hash<std::string>{}(key) % 128;
        kv_stores_[lock_index][key] = value;
    }

    file.close();
}

void ShardedHashStore::Save() {
    std::ofstream file(filename_);
    if (file.fail()) {
        throw std::runtime_error("Failed to open file for writing");
    }

    // 遍历哈希表，将每一个键值对写入文件
    for (auto& kvs : kv_stores_) {
        for (auto& kv : kvs) {
            file << kv.first << " " << kv.second << "\n";
        }
    }

    file.close();
    if (file.fail()) {
        throw std::runtime_error("Failed to write to file");
    }
}

void ShardedHashStore::Set(const std::string& key, const std::string& value) {
    size_t lock_index = std::hash<std::string>{}(key) % 128;
    std::lock_guard<std::mutex> lock(mutexes_[lock_index]);
    kv_stores_[lock_index][key] = value;
}

std::string ShardedHashStore::Get(const std::string& key) {
    size_t lock_index = std::hash<std::string>{}(key) % 128;
    std::lock_guard<std::mutex> lock(mutexes_[lock_index]);
    auto it = kv_stores_[lock_index].find(key);
    if (it != kv_stores_[lock_index].end()) {
        return it->second;
    } else {
        return "";
    }
}