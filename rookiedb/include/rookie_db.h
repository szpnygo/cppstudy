#pragma once

#include "extra_attributes.h"
#include "filter.h"
#include "table.h"
#include "vec_data.h"
#include "vector_database.h"

#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

class DataFilter : public SearchFilter {
  public:
    const std::string& name;
    std::shared_ptr<Filter> filter;
    std::shared_ptr<ExtraAttributes> extra_attributes_;

    DataFilter(const std::string& name,
               std::shared_ptr<Filter> filter,
               std::shared_ptr<ExtraAttributes> extra_attributes)
        : name(name),
          filter(filter),
          extra_attributes_(extra_attributes) {}

    bool operator()(uint64_t id) {
        auto data = extra_attributes_->Get(name, id);
        return filter->matchAttributes(data);
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
                     bool normalize = true);

    // check table exists
    bool hasTable(const std::string& name);

    // add vector data
    void add(const std::string& name, VecData& data);

    // update vector data
    void update(const std::string& name, uint64_t id, std::vector<float>& v);

    // erase vector data
    void erase(const std::string& name, uint64_t id);

    // get vector data
    VecData get(const std::string& name, uint64_t id);

    // check vector data exists
    bool exists(const std::string& name, uint64_t id);

    // get count of vector data
    size_t count(const std::string& name);

    // get count of deleted vector data
    size_t count_deleted(const std::string& name);

    // get internal id
    uint64_t getInternalID(const std::string& name, uint64_t id);

    // get max elements
    size_t getMaxElements(const std::string& name);

    // resize vector database
    void resize(const std::string& name, const size_t max_elements);

    // search vector data
    // return vector of (id, distance)
    // if filter is not null, filter the result
    std::vector<std::pair<uint64_t, float>>
    search(const std::string& name,
           std::vector<float>& v,
           const size_t k,
           std::shared_ptr<Filter> filter = nullptr);

  private:
    std::unordered_map<std::string, std::shared_ptr<VectorDatabase>> dbs_;
    std::unordered_map<std::string, std::shared_ptr<TableSchema>> schemas_;

    std::shared_ptr<ExtraAttributes> extra_attributes_;

    // get a vector database
    VectorDatabase& get(const std::string& name);
};