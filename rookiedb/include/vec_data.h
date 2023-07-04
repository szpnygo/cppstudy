#pragma once

#include "extra_attributes.h"

#include <iostream>
#include <vector>

class VecData {
  public:
    VecData(uint64_t id, std::vector<float>& v)
        : id(id),
          v(v),
          attributes(std::make_unique<Attributes>()) {}

    uint64_t id;
    std::vector<float> v;
    std::unique_ptr<Attributes> attributes;

    void setAttribute(const std::string& key, Value value) {
        (*attributes)[key] = value;
    }

    template <typename T>
    std::optional<T> getAttributeAs(const std::string& key) {
        auto it = attributes->find(key);
        if (it != attributes->end() && std::holds_alternative<T>(it->second)) {
            return std::get<T>(it->second);
        }
        return std::nullopt;
    }
};