#pragma once
#include <functional>
#include <memory>

namespace easywebrtc {

enum class ReadyState { Connecting = 0, Open = 1, Closing = 2, Closed = 3 };

class WebSocketClientImpl;

typedef std::function<void(const std::string &message, const bool binary)>
    OnMessage;
typedef std::function<void(const std::string &error)> OnError;
typedef std::function<void()> OnOpen;
typedef std::function<void(const std::string &reason)> OnClose;
typedef std::function<void()> OnPing;
typedef std::function<void()> OnPong;

class WebSocketsClient {
public:
  WebSocketsClient();
  ~WebSocketsClient();

  void connect(const std::string &url);
  void close();
  void stop();

  ReadyState getReadyState() const;

  void onOpen(OnOpen onOpen) { _onOpen = onOpen; }
  void onError(OnError onError) { _onError = onError; }
  void onClose(OnClose onClose) { _onClose = onClose; }
  void onMessage(OnMessage onMessage) { _onMessage = onMessage; }
  void onPing(OnPing onPing) { _onPing = onPing; }
  void onPong(OnPong onPong) { _onPong = onPong; }

  void sendMessage(const std::string &data, bool binary = false);

private:
  std::unique_ptr<WebSocketClientImpl> _impl;
  OnOpen _onOpen;
  OnError _onError;
  OnClose _onClose;
  OnMessage _onMessage;
  OnPing _onPing;
  OnPong _onPong;
};
}; // namespace easywebrtc
