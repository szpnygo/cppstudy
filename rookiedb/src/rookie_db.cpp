#include "rookie_db.h"

#include <algorithm>
#include <memory>

RookieDB::RookieDB() {}

RookieDB::~RookieDB() {}

void RookieDB::createTable(const std::string& name,
                           std::shared_ptr<TableSchema> schema,
                           const size_t dim,
                           const size_t max_elements,
                           const size_t M,
                           const size_t ef_construction,
                           bool normalize) {

    schemas_[name] = schema;
    dbs_[name] = std::make_shared<VectorDatabase>(
        name, dim, max_elements, M, ef_construction, normalize);
    extra_attributes_.create(name);
}

bool RookieDB::hasTable(const std::string& name) {
    return dbs_.find(name) != dbs_.end();
}

VectorDatabase& RookieDB::get(const std::string& name) {
    auto it = dbs_.find(name);
    if (it == dbs_.end()) {
        throw std::runtime_error("Table " + name + " not found");
    }

    return *it->second;
}

void RookieDB::add(const std::string& name, VecData& data) {
    // check vector length
    if (data.v.size() != get(name).getDim()) {
        throw std::runtime_error("Vector length mismatch");
    }
    // check data attributes against schema
    schemas_[name]->checkVecData(data);

    get(name).add(data.id, data.v);
    extra_attributes_.add(name, data.id, std::move(data.attributes));
}

VecData RookieDB::get(const std::string& name, uint64_t id) {
    auto v = get(name).get(id);
    auto e = extra_attributes_.Get(name, id);
    VecData data(id, v);
    data.attributes = std::make_unique<Attributes>(*e);
    return data;
}

size_t RookieDB::count(const std::string& name) { return get(name).count(); }

void RookieDB::update(const std::string& name,
                      uint64_t id,
                      std::vector<float>& v) {
    get(name).update(id, v);
}

void RookieDB::erase(const std::string& name, uint64_t id) {
    get(name).erase(id);
}

bool RookieDB::exists(const std::string& name, uint64_t id) {
    return get(name).exists(id);
}

size_t RookieDB::count_deleted(const std::string& name) {
    return get(name).count_deleted();
}

uint64_t RookieDB::getInternalID(const std::string& name, uint64_t id) {
    return get(name).getInternalID(id);
}

size_t RookieDB::getMaxElements(const std::string& name) {
    return get(name).getMaxElements();
}

void RookieDB::resize(const std::string& name, const size_t max_elements) {
    get(name).resizeIndex(max_elements);
}

std::vector<std::pair<uint64_t, float>>
RookieDB::search(const std::string& name,
                 std::vector<float>& v,
                 const size_t k,
                 SearchFilter* filter) {
    if (k > 10000000000) {
        throw std::runtime_error("k must be greater than 0");
    }
    return get(name).search(v, k, filter);
    std::vector<VecData> vec_result;
}
