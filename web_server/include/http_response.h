#pragma once
#include <iostream>
#include <map>
#include <optional>

struct HttpResponse {
    std::string version;                        // HTTP版本
    int status_code;                            // 状态码
    std::string status_text;                    // 状态码描述
    std::map<std::string, std::string> headers; // 响应头
    std::optional<std::string> body;            // 响应体

    static std::string response_to_string(const HttpResponse& response);
};