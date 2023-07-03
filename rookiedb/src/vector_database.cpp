#include "vector_database.h"

#include "hnswlib/hnswlib.h"

#include <vector>

VectorDatabase::VectorDatabase(const std::string& name,
                               const size_t dim,
                               const size_t max_elements,
                               const size_t M,
                               const size_t ef_construction,
                               bool normalize)
    : normalize_(normalize),
      name_(name),
      max_elements_(max_elements) {
    // init space and db, use cosine similarity, remember all then data should
    // be normalized with InnerProductSpace
    space_ = std::make_unique<hnswlib::InnerProductSpace>(dim);
    db_ = std::make_unique<hnswlib::HierarchicalNSW<float>>(
        space_.get(), max_elements, M, ef_construction);
};

std::string VectorDatabase::name() { return name_; }

void VectorDatabase::resizeIndex(size_t new_max_elements) {
    db_->resizeIndex(new_max_elements);
    max_elements_ = new_max_elements;
}

size_t VectorDatabase::getMaxElements() { return max_elements_; }

VectorDatabase::~VectorDatabase(){};

void VectorDatabase::normalize(std::vector<float>& v) {
    float sum = 0;
    for (const auto& val : v) {
        sum += val * val;
    }
    float norm = std::sqrt(sum);
    for (auto& val : v) {
        val /= norm;
    }
};

void VectorDatabase::add(uint64_t id, std::vector<float>& v) {
    if (normalize_) {
        normalize(v);
    }
    db_->addPoint(v.data(), id);
};

void VectorDatabase::del(uint64_t id) { db_->markDelete(id); };

void VectorDatabase::update(uint64_t id, std::vector<float>& v) {
    if (normalize_) {
        normalize(v);
    }
    db_->addPoint(v.data(), id);
}

std::vector<float> VectorDatabase::get(uint64_t id) {
    return db_->getDataByLabel<float>(id);
}

bool VectorDatabase::exists(uint64_t id) {
    auto search = db_->label_lookup_.find(id);
    if (search == db_->label_lookup_.end() ||
        db_->isMarkedDeleted(search->second)) {
        return false;
    }

    return true;
};

size_t VectorDatabase::count() {
    return db_->getCurrentElementCount() - db_->getDeletedCount();
};

size_t VectorDatabase::count_deleted() { return db_->getDeletedCount(); };

uint64_t VectorDatabase::getInternalID(uint64_t id) {
    auto search = db_->label_lookup_.find(id);
    if (search == db_->label_lookup_.end() ||
        db_->isMarkedDeleted(search->second)) {
        return 0;
    }

    return search->second;
};

std::vector<std::pair<uint64_t, float>> VectorDatabase::search(
    std::vector<float>& query_data, size_t k, SearchFilter* isIdAllowed) {

    if (normalize_) {
        normalize(query_data);
    }

    auto originalResult = db_->searchKnn(query_data.data(), k, isIdAllowed);

    std::vector<std::pair<uint64_t, float>> result;
    result.reserve(originalResult.size());

    while (!originalResult.empty()) {
        auto pair = originalResult.top();
        result.emplace_back(pair.second, pair.first);
        originalResult.pop();
    }

    std::reverse(result.begin(), result.end());

    return result;
};