#include "extra_attributes.h"

#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>

void ExtraAttributes::create(const std::string& name) {
    std::lock_guard<std::mutex> lock(global_mutex_);
    if (tables_.find(name) == tables_.end()) {
        tables_[name] = TableData();
        mutexes_[name] = std::make_shared<std::shared_mutex>();
    }
}

void ExtraAttributes::add(const std::string& name,
                          uint64_t id,
                          std::unique_ptr<Attributes> attrs) {
    std::unique_lock<std::shared_mutex> lock(*mutexes_[name]);
    tables_[name][id] = std::shared_ptr<const Attributes>(std::move(attrs));
}

void ExtraAttributes::erase(const std::string& name, uint64_t id) {
    std::lock_guard<std::shared_mutex> lock(*mutexes_[name]);
    tables_[name].erase(id);
}

std::shared_ptr<const Attributes> ExtraAttributes::Get(const std::string& table,
                                                       uint64_t id) {
    std::shared_lock<std::shared_mutex> lock(*mutexes_[table]);
    auto it = tables_[table].find(id);
    if (it != tables_[table].end()) {
        return it->second;
    }
    return nullptr;
}
