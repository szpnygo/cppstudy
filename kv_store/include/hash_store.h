#pragma once

#include "base.h"

#include <mutex>
#include <string>
#include <unordered_map>

class HashStore : public BaseStore {
  public:
    HashStore(const std::string& filename) : filename_(filename) {}

    std::string Get(const std::string& key) override;
    void Set(const std::string& key, const std::string& value) override;
    void Load() override;
    void Save() override;

  private:
    std::string filename_;
    std::mutex mtx_;
    std::unordered_map<std::string, std::string> kv_store_;
};