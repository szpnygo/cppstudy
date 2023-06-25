#pragma once

#include <map>
#include <string>

struct HttpRequest {
    std::string method;  // 请求方法，比如"GET"或"POST"
    std::string uri;     // 请求的URI，比如"/"
    std::string version; // HTTP协议版本，比如"HTTP/1.1"
    std::map<std::string, std::string> headers; // 请求头，键值对形式
    std::optional<std::string> body; // 请求体，可能不存在，所以用std::optional

    // 用于解析HTTP请求的函数，返回HttpRequest结构
    // 本函数会从HTTP请求字符串中解析出请求方法、URI、HTTP版本、请求头和请求体
    static std::optional<HttpRequest> parse_request(const std::string& request);
};
