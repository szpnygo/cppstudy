#include "hash_store.h"

#include <fstream>

void HashStore::Load() {
    std::ifstream file(filename_);
    if (file.fail()) {
        throw std::runtime_error("Failed to open file for reading");
    }
    std::string key;
    std::string value;

    // 从文件中读取键值对，直到文件结束
    while (file >> key >> value) {
        kv_store_[key] = value;
    }

    file.close();
}

void HashStore::Save() {
    std::ofstream file(filename_);
    if (file.fail()) {
        throw std::runtime_error("Failed to open file for writing");
    }

    // 遍历哈希表，将每一个键值对写入文件
    for (auto& kv : kv_store_) {
        file << kv.first << " " << kv.second << "\n";
    }

    file.close();
    if (file.fail()) {
        throw std::runtime_error("Failed to write to file");
    }
}

void HashStore::Set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx_);
    kv_store_[key] = value;
}

std::string HashStore::Get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = kv_store_.find(key);
    if (it != kv_store_.end()) {
        return it->second;
    } else {
        return "";
    }
}