#include "http_server.h"

#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>

#define MAX_BUFFER_SIZE 30000

HttpResponse handle_not_found(const HttpRequest& request) {
    HttpResponse response;
    response.version = "HTTP/1.1";
    response.status_code = 404;
    response.status_text = "Not Found";
    response.headers = {{"Content-Type", "text/plain"}};
    return response;
}

void HttpServer::add_route(const std::string& method,
                           const std::string& uri,
                           HttpServer::HandlerType handler) {
    route_map_[{method, uri}] = handler;
}

// 开始服务
void HttpServer::start() {
    // 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        std::cerr << "Failed to create socket!" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    // 将套接字绑定到本地地址和端口
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "\nWaiting for a connection...\n";

    while (true) {

        // 接受新连接
        socklen_t addrlen = sizeof(address);
        int new_socket =
            accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // 对于每个新连接，创建一个新线程来处理
        threads_.emplace_back(&HttpServer::handle_connection, this, new_socket);
    }

    // 在程序退出前，等待所有线程完成
    for (auto& thread : threads_) {
        thread.join();
    }
}

void HttpServer::handle_connection(int new_socket) {
    // 读取请求
    char buffer[MAX_BUFFER_SIZE] = {0};
    long valread = read(new_socket, buffer, MAX_BUFFER_SIZE);
    std::cout << "\nReceived:\n" << buffer << std::endl;

    // 解析请求
    auto request = HttpRequest::parse_request(std::string(buffer));
    std::string response_string;
    if (!request) {
        // 如果请求解析失败，发送400 Bad Request响应
        HttpResponse response;
        response.version = "HTTP/1.1";
        response.status_code = 400;
        response.status_text = "Bad Request";
        response.headers = {{"Content-Type", "text/plain"}};
        response_string = HttpResponse::response_to_string(response);
    } else {
        // 根据请求方法和URI查找对应的处理函数
        auto route_key = std::make_pair(request->method, request->uri);
        auto route_iter = route_map_.find(route_key);
        if (route_iter != route_map_.end()) {
            // 如果找到了处理函数，调用它来生成响应
            HttpResponse response = route_iter->second(*request);
            response_string = HttpResponse::response_to_string(response);
        } else {
            // 如果没有找到处理函数，发送404 Not Found响应
            HttpResponse response = handle_not_found(*request);
            response_string = HttpResponse::response_to_string(response);
        }
    }

    // 发送响应
    write(new_socket, response_string.c_str(), response_string.length());

    // 关闭连接
    close(new_socket);
}