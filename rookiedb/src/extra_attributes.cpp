#include "extra_attributes.h"

#include <mutex>
#include <optional>
#include <shared_mutex>

ExtraAttributes::ExtraAttributes() {}

ExtraAttributes::~ExtraAttributes() {}

std::shared_ptr<std::shared_mutex>
ExtraAttributes::getMutex(const uint64_t key) {
    std::lock_guard<std::mutex> lock(global_mutex_);
    auto [it, inserted] =
        mutexes_.emplace(key, std::make_shared<std::shared_mutex>());
    return it->second;
}

void ExtraAttributes::insert(uint64_t key,
                             const std::string& subkey,
                             Value value) {
    std::unique_lock<std::shared_mutex> lock(*getMutex(key));
    storage_[key][subkey] = std::move(value);
}

void ExtraAttributes::insert(uint64_t key,
                             const std::string& subkey,
                             std::unordered_map<std::string, Value>& values) {
    std::unique_lock<std::shared_mutex> lock(*getMutex(key));
    for (const auto& [k, v] : values) {
        storage_[key][subkey] = v;
    }
}

std::optional<Value> ExtraAttributes::get(uint64_t key,
                                          const std::string& subkey) {
    std::shared_lock<std::shared_mutex> lock(*getMutex(key));
    auto it1 = storage_.find(key);
    if (it1 == storage_.end()) {
        return std::nullopt;
    }

    auto it2 = it1->second.find(subkey);
    if (it2 == it1->second.end()) {
        return std::nullopt;
    }

    return it2->second;
}

void ExtraAttributes::erase(uint64_t key, const std::string& subkey) {
    std::unique_lock<std::shared_mutex> lock(*getMutex(key));
    auto it1 = storage_.find(key);
    if (it1 == storage_.end()) {
        return;
    }

    it1->second.erase(subkey);

    if (it1->second.empty()) {
        storage_.erase(it1);
    }
}