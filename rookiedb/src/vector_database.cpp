#include "vector_database.h"

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
    space_ = new hnswlib::InnerProductSpace(dim);
    db_ = new hnswlib::HierarchicalNSW<float>(
        space_, max_elements, M, ef_construction);
};

std::string VectorDatabase::name() { return name_; }

void VectorDatabase::resizeIndex(size_t new_max_elements) {
    db_->resizeIndex(new_max_elements);
    max_elements_ = new_max_elements;
}

size_t VectorDatabase::getMaxElements() { return max_elements_; }

VectorDatabase::~VectorDatabase() {
    delete space_;
    delete db_;
};

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