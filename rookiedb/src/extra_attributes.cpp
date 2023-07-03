#include "extra_attributes.h"

#include <mutex>
#include <optional>
#include <shared_mutex>

ExtraAttributes::ExtraAttributes() {}

ExtraAttributes::~ExtraAttributes() {}

void ExtraAttributes::insert(const std::string& key,
                             const std::string& subkey,
                             ExtraAttributes::Value value) {
    std::unique_lock<std::shared_mutex> lock(mutexes_[key]);
    storage_[key][subkey] = value;
}

std::optional<ExtraAttributes::Value>
ExtraAttributes::get(const std::string& key, const std::string& subkey) {
    std::shared_lock<std::shared_mutex> lock(mutexes_[key]);
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

void ExtraAttributes::erase(const std::string& key, const std::string& subkey) {
    std::unique_lock<std::shared_mutex> lock(mutexes_[key]);
    auto it1 = storage_.find(key);
    if (it1 == storage_.end()) {
        return;
    }

    it1->second.erase(subkey);

    if (it1->second.empty()) {
        storage_.erase(it1);
    }
}