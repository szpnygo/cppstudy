#include "app.h"
#include "rtc_data_channel.h"
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace easywebrtc;

class AppTest : public ::testing::Test {
protected:
  easywebrtc::WebRTCApp app;
};

TEST_F(AppTest, CreatePeerConnection) {
  RTCConfig config{
      .iceServers =
          {
              {"stun:stun.l.google.com:19302"},
              {"stun:stun.l.google.com:19302"},
          },
  };
  auto pc = app.createPeerConnection(config);
}