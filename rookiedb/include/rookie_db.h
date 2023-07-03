#pragma once

#include "extra_attributes.h"
#include "vector_database.h"

#include <memory>
#include <unordered_map>
#include <vector>

class VecData {
  public:
    VecData(uint64_t id, std::vector<float>& v) : id(id), v(v) {}

    uint64_t id;
    std::vector<float> v;
    std::unordered_map<std::string, Value> attributes;

    void setAttribute(const std::string& key, Value value) {
        attributes[key] = value;
    }
};

class RookieDB {
  public:
    RookieDB();

    ~RookieDB();

    // create vector database
    void createTable(const std::string& name,
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

    // get a vector database
    VectorDatabase& get(const std::string& name);
};