#pragma once

#include "vector_database.h"

#include <unordered_map>
#include <vector>

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

  private:
    std::unordered_map<std::string, VectorDatabase> dbs_;

    // get a vector database
    VectorDatabase& getDB(const std::string& name);
};