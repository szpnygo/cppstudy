#pragma once

#include <iostream>
#include <string>

class ApiRequest {
  public:
    ApiRequest() = default;

    static std::string request(const std::string& url);
};