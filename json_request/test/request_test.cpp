#include "request.h"

#include <chrono>
#include <gtest/gtest.h>

class ApiRequestTest : public ::testing::Test {
  protected:
    ApiRequest request;

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(ApiRequestTest, CheckRequest) {
    std::string url = "https://api.github.com/users/github";
    auto start = std::chrono::system_clock::now();
    std::string result = request.request(url);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    EXPECT_TRUE(result.find("9919"));
    EXPECT_LE(elapsed_seconds.count(), 3.0);
    EXPECT_EQ(request.get_counter(), 1);
}

TEST(ApiRequest, CheckResult) {
    ApiRequest request;
    std::string url = "https://api.github.com/users/github";
    auto start = std::chrono::system_clock::now();
    std::string result = request.request(url);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    EXPECT_TRUE(result.find("9919"));
    EXPECT_LE(elapsed_seconds.count(), 3.0);
    EXPECT_EQ(request.get_counter(), 1);
};