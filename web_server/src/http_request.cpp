#include "http_request.h"

#include <iostream>
#include <sstream>
#include <string>

// 用于解析HTTP请求的函数，返回HttpRequest结构
// 本函数会从HTTP请求字符串中解析出请求方法、URI、HTTP版本、请求头和请求体
std::optional<HttpRequest>
HttpRequest::parse_request(const std::string& request) {
    // 定义HttpRequest结构用于存储解析结果
    HttpRequest result;
    std::istringstream iss(request);

    std::string line;
    std::getline(iss, line);
    std::istringstream first_line(line);
    // 从请求行中解析出请求方法、URI和HTTP版本
    if (!(first_line >> result.method >> result.uri >> result.version)) {
        return std::nullopt;
    }

    // 解析请求头，每一行一个键值对
    while (std::getline(iss, line) && line != "\r") {
        auto sep_pos = line.find(':');
        if (sep_pos == std::string::npos) {
            return std::nullopt;
        }
        auto key = line.substr(0, sep_pos);
        auto value = line.substr(sep_pos + 2,
                                 line.size() - sep_pos - 3); // 去掉': '和'\r'
        result.headers[key] = value;
    }

    // 解析请求体
    std::getline(iss, line); // 跳过空行
    if (iss) {
        std::getline(iss, line, '\0'); // 读取请求体
        result.body = line;
    }

    return result;
}