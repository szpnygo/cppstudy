#pragma once
#include "http_request.h"
#include "http_response.h"

#include <iostream>
#include <map>
#include <vector>

class HttpServer {
  public:
    using HandlerType = std::function<HttpResponse(HttpRequest)>;

    HttpServer(int port) : port_(port) {}

    // 启动HTTP服务器，开始监听HTTP请求
    void start();

    // 添加路由，method是HTTP方法，比如"GET"或"POST"，path是请求路径，比如"/"
    void add_route(const std::string& method,
                   const std::string& path,
                   HandlerType handler);

  private:
    int port_;

    // 处理一个新的连接
    void handle_connection(int new_socket);

    // 路由表，键是(method, path)对，值是处理函数
    std::map<std::pair<std::string, std::string>,
             std::function<HttpResponse(HttpRequest)>>
        route_map_;

    std::vector<std::thread> threads_;
};