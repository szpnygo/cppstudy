#pragma once

#include <iostream>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <variant>

class ExtraAttributes {
  public:
    ExtraAttributes();

    ~ExtraAttributes();

    using Value = std::variant<int, std::string>;

    void insert(const std::string& key, const std::string& subkey, Value value);

    std::optional<Value> get(const std::string& key, const std::string& subkey);

    void erase(const std::string& key, const std::string& subkey);

  private:
    std::unordered_map<std::string, std::unordered_map<std::string, Value>>
        storage_;
    std::unordered_map<std::string, std::shared_mutex> mutexes_;
};