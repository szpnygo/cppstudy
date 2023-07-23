#pragma once
#include <functional>
#include <memory>

namespace easywebrtc {

enum class ReadyState { Connecting = 0, Open = 1, Closing = 2, Closed = 3 };

class WebSocketClientImpl;

typedef std::function<void(const std::string &message)> OnMessage;
typedef std::function<void(const std::string &error)> OnError;
typedef std::function<void()> OnOpen;
typedef std::function<void(const std::string &reason)> OnClose;
typedef std::function<void()> OnPing;
typedef std::function<void()> OnPong;

class WebSocketClient {
public:
  WebSocketClient();
  ~WebSocketClient();

  void connect(const std::string &url);
  void close();

  ReadyState getReadyState() const;

  void onOpen(OnOpen onOpen) { _onOpen = onOpen; }
  void onError(OnError onError) { _onError = onError; }
  void onClose(OnClose onClose) { _onClose = onClose; }
  void onMessage(OnMessage onMessage) { _onMessage = onMessage; }
  void onPing(OnPing onPing) { _onPing = onPing; }
  void onPong(OnPong onPong) { _onPong = onPong; }

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
