#include "websockets.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using namespace easywebrtc;
using json = nlohmann::json;

class WebSocketsClientTest : public ::testing::Test {
protected:
  WebSocketsClient client;
};

TEST_F(WebSocketsClientTest, Connect) {
  std::atomic_bool successCalled(false);
  client.onOpen([&successCalled]() { successCalled = true; });
  client.connect("wss://api.desktop.test.tcode.ltd/base");

  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(successCalled);
}

TEST_F(WebSocketsClientTest, Close) {
  std::atomic_bool successCalled(false);
  client.onOpen([this]() { client.close(); });
  client.onClose(
      [&successCalled](const std::string &reason) { successCalled = true; });
  client.connect("wss://api.desktop.test.tcode.ltd/base");

  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(successCalled);
}

TEST_F(WebSocketsClientTest, SendMessage) {
  json createMessage;
  createMessage["action"] = "CreateRemoteRoom";
  createMessage["rid"] = "rid_test";
  createMessage["uid"] = "uid_test";

  OnOpen onOpen = [this, &createMessage]() {
    client.sendMessage(createMessage.dump());
  };
  OnMessage onMessage = [](const std::string &message, bool isBinary) {
    auto result = json::parse(message);
    EXPECT_EQ(result["action"], "CreateRemoteRoomResponse");
  };

  client.onOpen(onOpen);
  client.onMessage(onMessage);
  client.connect("wss://api.desktop.test.tcode.ltd/base");

  std::this_thread::sleep_for(std::chrono::seconds(2));
}