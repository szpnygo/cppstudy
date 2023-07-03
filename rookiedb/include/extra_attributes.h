#pragma once

#include <iostream>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <variant>

using Value = std::variant<int, float, long, uint64_t, std::string>;

class ExtraAttributes {
  public:
    ExtraAttributes();

    ~ExtraAttributes();

    void initKey(const uint64_t key);

    void insert(uint64_t key, const std::string& subkey, Value value);

    void insert(uint64_t key,
                const std::string& subkey,
                std::unordered_map<std::string, Value>& values);

    std::optional<Value> get(uint64_t key, const std::string& subkey);

    void erase(uint64_t key, const std::string& subkey);

    std::shared_ptr<std::shared_mutex> getMutex(const uint64_t key);

  private:
    std::unordered_map<uint64_t, std::unordered_map<std::string, Value>>
        storage_;
    std::unordered_map<uint64_t, std::shared_ptr<std::shared_mutex>> mutexes_;

    std::mutex global_mutex_;
};