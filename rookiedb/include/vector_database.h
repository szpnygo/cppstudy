#pragma once

#include "hnswlib/hnswalg.h"
#include "hnswlib/hnswlib.h"
#include "hnswlib/space_ip.h"

#include <_types/_uint64_t.h>
#include <cstddef>
#include <string>
#include <vector>

class SearchFilter : public hnswlib::BaseFilterFunctor {
  public:
    bool operator()(hnswlib::labeltype label_id) override {
        return operator()(static_cast<uint64_t>(label_id));
    }

    virtual bool operator()(uint64_t id) { return true; }
};

class VectorDatabase {
  public:
    VectorDatabase(const std::string& name,
                   const size_t dim,
                   const size_t max_elements,
                   const size_t M = 16,
                   const size_t ef_construction = 200,
                   bool normalize = false);

    ~VectorDatabase();

    // get the name of the database
    std::string name();

    // add a vector to the database
    // id: the id of the vector, should be unique
    // v: the vector
    void add(uint64_t id, std::vector<float>& v);

    // update a vector in the database
    void update(uint64_t id, std::vector<float>& v);

    // remove a vector from the database
    void erase(uint64_t id);

    // get a vector from the database
    // id: the id of the vector
    std::vector<float> get(uint64_t id);

    // check if a vector exists in the database
    bool exists(uint64_t id);

    // return the number of vectors in the database
    size_t count();

    // return the number of deleted vectors in the database
    size_t count_deleted();

    // return the internal id of a vector
    uint64_t getInternalID(uint64_t id);

    // return the maximum number of vectors in the database
    size_t getMaxElements();

    // resize the database
    void resizeIndex(size_t new_max_elements);

    // search the database
    std::vector<std::pair<uint64_t, float>>
    search(std::vector<float>& query_data,
           size_t k,
           SearchFilter* isIdAllowed = nullptr);

    size_t getDim();

  private:
    std::string name_;
    bool normalize_;
    size_t max_elements_;
    size_t dim_;
    std::unique_ptr<hnswlib::InnerProductSpace> space_;
    std::unique_ptr<hnswlib::HierarchicalNSW<float>> db_;

    void normalize(std::vector<float>& v);
};