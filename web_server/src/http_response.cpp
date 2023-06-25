#include "http_response.h"

#include <sstream>

// 将HttpResponse结构转换为字符串
std::string HttpResponse::response_to_string(const HttpResponse& response) {
    std::ostringstream oss;
    oss << response.version << " " << response.status_code << " "
        << response.status_text << "\n";
    for (const auto& header : response.headers) {
        oss << header.first << ": " << header.second << "\n";
    }
    oss << "\n"; // headers和body之间的空行
    if (response.body) {
        oss << *response.body;
    }
    return oss.str();
}