#pragma once

#include "extra_attributes.h"
#include "vector_database.h"

#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
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

            if (it->second != &typeid(value)) {
                throw std::runtime_error(
                    "Key " + key + " has wrong type in schema " +
                    it->second->name() + " != " + typeid(value).name());
            }
        }
    }
};

class RookieDB {
  public:
    RookieDB();

    ~RookieDB();

    // create vector database
    void createTable(const std::string& name,
                     std::shared_ptr<TableSchema> schema,
                     const size_t dim,
                     const size_t max_elements,
                     const size_t M = 16,
                     const size_t ef_construction = 200,
                     bool normalize = false);

    bool hasTable(const std::string& name);

    void add(const std::string& name, VecData& data);

    void update(const std::string& name, uint64_t id, std::vector<float>& v);

    void erase(const std::string& name, uint64_t id);

    VecData get(const std::string& name, uint64_t id);

    bool exists(const std::string& name, uint64_t id);

    size_t count(const std::string& name);

    size_t count_deleted(const std::string& name);

    uint64_t getInternalID(const std::string& name, uint64_t id);

    size_t getMaxElements(const std::string& name);

    void resize(const std::string& name, const size_t max_elements);

    std::vector<std::pair<uint64_t, float>>
    search(const std::string& name,
           std::vector<float>& v,
           const size_t k,
           SearchFilter* filter = nullptr);

  private:
    std::unordered_map<std::string, std::shared_ptr<VectorDatabase>> dbs_;
    std::unordered_map<std::string, std::shared_ptr<TableSchema>> schemas_;

    // get a vector database
    VectorDatabase& get(const std::string& name);

    ExtraAttributes extra_attributes_;
};