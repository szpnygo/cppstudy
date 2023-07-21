#include "app.h"
#include "types.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace easywebrtc;

class AppTest : public ::testing::Test {
protected:
  easywebrtc::WebRTCApp app;
};

TEST_F(AppTest, createPeerConnection) {
  RTCConfig config{
      .iceServers =
          {
              {"stun:stun.l.google.com:19302"},
              {"stun:stun.l.google.com:19302"},
          },
  };
  auto pc = app.createPeerConnection(config);

  std::cout << "Hello, world!" << std::endl;
}