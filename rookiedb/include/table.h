#pragma once

#include "extra_attributes.h"
#include "vec_data.h"

#include <iostream>
#include <typeinfo>
#include <unordered_map>

class TableSchema {
  public:
    TableSchema() = default;
    // int, float, long, uint64_t, std::string
    std::unordered_map<std::string, const std::type_info*> schema;

    void addString(const std::string& key) {
        schema[key] = &typeid(std::string);
    }

    void addInt(const std::string& key) { schema[key] = &typeid(int); }

    void addFloat(const std::string& key) { schema[key] = &typeid(float); }

    void addLong(const std::string& key) { schema[key] = &typeid(long); }

    void addUint64(const std::string& key) { schema[key] = &typeid(uint64_t); }

    void checkVecData(VecData& data) {
        for (auto& [key, value] : *data.attributes) {
            auto it = schema.find(key);
            if (it == schema.end()) {
                throw std::runtime_error("Key " + key + " not found in schema");
            }

            const std::type_info& expectedType = *it->second;
            const std::string& keyRef = key;
            const std::type_info& expectedTypeRef = expectedType;

            std::visit(
                [&](auto&& arg) {
                    const std::type_info& valueType = typeid(arg);
                    if (expectedTypeRef != valueType) {
                        throw std::runtime_error("Key " + keyRef +
                                                 " has wrong type in schema " +
                                                 expectedTypeRef.name() +
                                                 " != " + typeid(arg).name());
                    }
                },
                value);
        }
    }
};