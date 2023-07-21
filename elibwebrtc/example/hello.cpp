#include "app.h"
#include <iostream>

int main() {
  std::cout << "Hello, world!" << std::endl;

  easywebrtc::WebRTCApp app;
  RTCConfig config{
      {
          {"stun:stun.l.google.com:19302"},
          {"stun:stun.l.google.com:19302"},
      },
  };
  auto pc = app.createPeerConnection(config);

  return 0;
}