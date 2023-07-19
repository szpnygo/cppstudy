#include "libwebrtc.h"
#include <iostream>

int main() {
  std::cout << "Hello, world!" << std::endl;

  libwebrtc::LibWebRTC::Initialize();

  return 0;
}