#include "websockets.h"
#include "ixwebsocket/IXNetSystem.h"
#include <ixwebsocket/IXWebSocket.h>
#include <memory>

namespace easywebrtc {

class WebSocketClientImpl {
public:
  WebSocketClientImpl() : webSocket(std::make_shared<ix::WebSocket>()) {
    ix::initNetSystem();
  };
  std::shared_ptr<ix::WebSocket> webSocket;
};

WebSocketsClient::WebSocketsClient()
    : _impl(std::make_unique<WebSocketClientImpl>()){};

WebSocketsClient::~WebSocketsClient() = default;

void WebSocketsClient::close() {
  _impl->webSocket->disableAutomaticReconnection();
  _impl->webSocket->close();
  ix::uninitNetSystem();
}

void WebSocketsClient::stop() {
  _impl->webSocket->stop();
  ix::uninitNetSystem();
}

ReadyState WebSocketsClient::getReadyState() const {
  return static_cast<ReadyState>(_impl->webSocket->getReadyState());
}

void WebSocketsClient::connect(const std::string &url) {
  _impl->webSocket->setTLSOptions({});
  _impl->webSocket->setUrl(url);
  _impl->webSocket->setOnMessageCallback(
      [this](const ix::WebSocketMessagePtr &msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
          if (_onMessage) {
            _onMessage(msg->str, msg->binary);
          }
        } else if (msg->type == ix::WebSocketMessageType::Open) {
          if (_onOpen) {
            _onOpen();
          }
        } else if (msg->type == ix::WebSocketMessageType::Error) {
          if (_onError) {
            _onError(msg->errorInfo.reason);
          }
        } else if (msg->type == ix::WebSocketMessageType::Close) {
          if (_onClose) {
            _onClose(msg->closeInfo.reason);
          }
        } else if (msg->type == ix::WebSocketMessageType::Ping) {
          if (_onPing) {
            _onPing();
          }
        } else if (msg->type == ix::WebSocketMessageType::Pong) {
          if (_onPong) {
            _onPong();
          }
        }
      });
  _impl->webSocket->start();
}

bool WebSocketsClient::sendMessage(const std::string &data, bool binary) {
  auto result = _impl->webSocket->send(data, binary);
  return result.success;
}

}; // namespace easywebrtc