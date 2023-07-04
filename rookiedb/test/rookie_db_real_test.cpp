#include "filter.h"
#include "rookie_db.h"
#include "vec_data.h"
#include "vector_database.h"

#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class RookieDBTest : public ::testing::Test {
  protected:
    RookieDB* db;
    std::vector<VecData> items;

    virtual void SetUp() override {
        std::ifstream file("../../test/data.txt");

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string index, key, embedding;
            if (std::getline(iss, index, ':') && std::getline(iss, key, ':') &&
                std::getline(iss, embedding)) {

                std::vector<float> embedding_vec;
                std::istringstream iss2(embedding);
                std::string embedding_value;
                while (std::getline(iss2, embedding_value, ',')) {
                    embedding_vec.push_back(std::stof(embedding_value));
                }
                VecData data(std::stoi(index), embedding_vec);
                data.setAttribute("key", Value(key));
                data.setAttribute("id", Value(uint64_t(std::stoi(index))));
                items.push_back(std::move(data));
            }
        }

        db = new RookieDB();
    }

    virtual void TearDown() override { delete db; }
};

TEST_F(RookieDBTest, RealData) {
    std::shared_ptr<TableSchema> schema = std::make_shared<TableSchema>();
    schema->addString("key");
    schema->addUint64("id");
    db->createTable("data", schema, 1536, 200, 32, 200, true);

    for (auto& item : items) {
        db->add("data", item);
    }

    ASSERT_EQ(db->count("data"), 189);

    // get item with id = 1
    auto data = db->get("data", 1);
    ASSERT_EQ(data.id, 1);
    ASSERT_EQ(data.getAttributeAs<std::string>("key"), "水果");

    // filter with id < 50
    std::shared_ptr<Filter> filter = std::make_shared<Filter>();
    filter->setLessThan("id", Value(uint64_t(50)));
    std::vector<float> embedding = data.v;
    auto results = db->search("data", embedding, 100, filter);
    ASSERT_EQ(results.size(), 49);

    // search 水果 and find it in top3
    std::vector<float> embedding2 = data.v;
    auto results2 = db->search("data", embedding2, 20);
    ASSERT_EQ(results2.size(), 20);

    for (auto result : results2) {
        auto info = db->get("data", result.first);
        auto key = info.getAttributeAs<std::string>("key").value();
        std::cout << result.first << " " << key << " " << result.second
                  << std::endl;
    }

    bool found;
    for (size_t i = 0; i < 3; i++) {
        if (results2[i].first == 1) {
            found = true;
            break;
        }
    }

    ASSERT_TRUE(found);
}