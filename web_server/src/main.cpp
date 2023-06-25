#include "http_request.h"
#include "http_response.h"
#include "http_server.h"

#include <arpa/inet.h>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define PORT 8080

// 修改：路由处理函数，接收一个HttpRequest，返回一个HttpResponse
HttpResponse handle_root(const HttpRequest& request) {
    HttpResponse response;
    response.version = "HTTP/1.1";
    response.status_code = 200;
    response.status_text = "OK";
    response.headers = {{"Content-Type", "text/plain"}};
    if (request.body) {
        response.body = "Received body: " + *request.body;
    } else {
        response.body = "Hello, root!";
    }
    return response;
}

HttpResponse handle_other(const HttpRequest& request) {
    HttpResponse response;
    response.version = "HTTP/1.1";
    response.status_code = 200;
    response.status_text = "OK";
    response.headers = {{"Content-Type", "text/plain"}};
    response.body = "Hello, other!";
    return response;
}

HttpResponse handle_new_route(const HttpRequest& request) {
    HttpResponse response;
    response.version = "HTTP/1.1";
    response.status_code = 200;
    response.status_text = "OK";
    response.headers = {{"Content-Type", "text/plain"}};
    response.body = "Hello, new route!";
    return response;
}

int main() {
    HttpServer server(8080);

    server.add_route("GET", "/", handle_root);
    server.add_route("GET", "/other", handle_other);
    server.add_route("POST", "/new_route", handle_new_route);
    // 添加更多路由...

    server.start();

    return 0;
}
