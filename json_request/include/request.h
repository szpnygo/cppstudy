#pragma once

#include <atomic>
#include <iostream>
#include <string>

class ApiRequest {
  public:
    ApiRequest() : counter_(0){};

    std::string request(const std::string& url);

    int get_counter() const { return counter_.load(); }

  private:
    std::mutex cout_mutex_;    // 保护 std::cout 的锁
    std::atomic<int> counter_; // 计数器
};