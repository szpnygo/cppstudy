#include "app.h"
#include "websockets.h"
#include <iostream>

int main() {
  std::cout << "Hello, world!" << std::endl;

  easywebrtc::WebRTCApp app;

  app.hello();

  easywebrtc::WebSocketClient client;

  return 0;
}