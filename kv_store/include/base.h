#pragma once

#include <string>

class BaseStore {
  public:
    virtual std::string Get(const std::string& key) = 0;
    virtual void Set(const std::string& key, const std::string& value) = 0;
    virtual void Load() = 0;
    virtual void Save() = 0;
    virtual ~BaseStore() {}
};