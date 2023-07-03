#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <variant>

using Value = std::variant<int, float, long, uint64_t, std::string>;

using Attributes = std::unordered_map<std::string, Value>;

using TableData =
    std::unordered_map<uint64_t, std::shared_ptr<const Attributes>>;

class ExtraAttributes {
  public:
    ExtraAttributes() = default;

    void create(const std::string& name);

    void add(const std::string& name,
             uint64_t id,
             std::unique_ptr<Attributes> attrs);

    void erase(const std::string& name, uint64_t id);

    std::shared_ptr<const Attributes> Get(const std::string& table,
                                          uint64_t id);

  private:
    std::unordered_map<std::string, TableData> tables_;
    std::unordered_map<std::string, std::shared_ptr<std::shared_mutex>>
        mutexes_;
    std::mutex global_mutex_;
};