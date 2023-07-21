#include "app.h"
#include "peerconnection.h"
#include "session_description.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace easywebrtc;

class AppTest : public ::testing::Test {
protected:
  easywebrtc::WebRTCApp app;
  RTCConfig config{
      .iceServers =
          {
              {"stun:stun.l.google.com:19302"},
              {"stun:stun.l.google.com:19302"},
          },
  };
};

TEST_F(AppTest, CreatePeerConnection) {
  auto pc = app.createPeerConnection(config);
}

TEST_F(AppTest, CreateOffer) {
  auto pc = app.createPeerConnection(config);
  std::atomic_bool successCalled(false);
  OnCreateSuccess success = [&successCalled](const SessionDescription &sdp) {
    successCalled = true;
  };

  pc->createOffer(success, [](const std::string &error) {});

  std::this_thread::sleep_for(std::chrono::seconds(1));

  EXPECT_TRUE(successCalled);
}

TEST_F(AppTest, SetDescription) {
  auto pc = app.createPeerConnection(config);
  std::atomic_bool successCalled(false);
  OnCreateSuccess success = [pc,
                             &successCalled](const SessionDescription &sdp) {
    OnSetSuccess setSuccess = [&successCalled]() { successCalled = true; };
    pc->setLocalDescription(sdp, setSuccess, [](const std::string &error) {});
  };

  pc->createOffer(success, [](const std::string &error) {});

  std::this_thread::sleep_for(std::chrono::seconds(1));

  EXPECT_TRUE(successCalled);
}