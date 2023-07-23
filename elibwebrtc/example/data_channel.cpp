#include "app.h"
#include "candidate.h"
#include "peerconnection.h"
#include "spdlog/spdlog.h"
#include "websockets.h"
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace easywebrtc;

const std::string &kServerUrl = "wss://api.desktop.test.tcode.ltd/base";
// const std::string &kServerUrl = "ws://127.0.0.1:8888/base";

std::mutex _globalMutex;
std::condition_variable _roomCreatedVar;
bool _roomCreated = false;

void clientA() {
  spdlog::info("clientA start");
  std::mutex _mutex;
  std::condition_variable cv;
  bool taskFinish = false;

  WebRTCApp app;
  WebSocketsClient client;
  std::shared_ptr<PeerConnection> pc;
  std::shared_ptr<DataChannel> dc;

  OnOpen onOpen = [&client, &taskFinish, &cv]() {
    spdlog::info("clientA WebSockets onOpen");
    // 创建房间
    json createMessage;
    createMessage["action"] = "CreateRemoteRoom";
    createMessage["rid"] = "rid_test";
    createMessage["uid"] = "clientA";

    auto result = client.sendMessage(createMessage.dump());
    if (!result) {
      spdlog::error("clientA WebSockets sendMessage error");
      return;
    }
    // 等待房间创建成功
    spdlog::info("clientA Create Room");
  };
  OnClose onClose = [](const std::string &reason) {
    spdlog::info("clientA WebSockets onClose %s", reason);
  };
  OnError onError = [](const std::string &error) {
    spdlog::error("clientA WebSockets onError %s", error);
  };

  std::function<void(bool)> onCreateRemoteRoomResponse = [](bool isOk) {
    if (isOk) {
      spdlog::info("clientA Crate Room ok");
      // 创建房间成功
      std::unique_lock<std::mutex> lock(_globalMutex);
      _roomCreated = true;
      _roomCreatedVar.notify_one();
    } else {
      spdlog::error("clientA Crate Room fail");
    }
  };

  std::function<void()> onJoinNotify = [&app, &pc, &dc, &client]() {
    spdlog::info("clientA Join Room Notify");
    spdlog::info("<--------- start WebRTC --------->");
    // 创建offer
    pc = app.createPeerConnection({});
    pc->onIceConnectionState([](IceConnectionState state) {
      spdlog::info("clientA onIceConnectionState %d", state);
    });
    pc->onIceCandidate(
        [](ICECandidate candidate) { spdlog::info("clientA onIceCandidate"); });
    pc->onPeerConnectionState([](PeerConnectionState state) {
      spdlog::info("clientA onPeerConnectionState %d", state);
    });

    dc = pc->createDataChannel("test");

    pc->createOffer(
        [&pc, &client](const SessionDescription &offer) {
          spdlog::info("clientA createOffer success");
          pc->setLocalDescription(
              offer,
              [&offer, &client]() {
                spdlog::info("clientA setLocalDescription success");

                // 发送offer给clientB
                json offerMessage;
                offerMessage["action"] = "SendRTCData";
                offerMessage["rid"] = "rid_test";
                offerMessage["uid"] = "clientB";
                offerMessage["from"] = "clientA";
                offerMessage["type"] = "offer";
                offerMessage["data"] = "offer.sdp()";

                auto result = client.sendMessage(offerMessage.dump());
                if (result) {
                  spdlog::info("clientA Send Offer success");
                } else {
                  spdlog::error("clientA Send Offer error");
                }
              },
              [](const std::string &error) {
                spdlog::error("clientA setLocalDescription error %s", error);
              });
        },
        [](const std::string &error) {
          spdlog::error("clientA createOffer error %s", error);
        });
    // create data channel
  };

  OnMessage onMessage = [onCreateRemoteRoomResponse, onJoinNotify](
                            const std::string &message, bool isBinary) {
    auto result = json::parse(message);
    spdlog::info("clientA onMessage {}", result["action"]);
    if (result["action"] == "CreateRemoteRoomResponse") {
      onCreateRemoteRoomResponse(result["ok"]);
      return;
    } else if (result["action"] == "JoinNotify") {
      spdlog::info("clientA JoinNotify from {}", result["uid"]);
      onJoinNotify();
      return;
    }
  };

  client.onOpen(onOpen);
  client.onClose(onClose);
  client.onError(onError);
  client.onMessage(onMessage);
  client.connect(kServerUrl);

  // next step
  std::unique_lock<std::mutex> lock(_mutex);
  cv.wait(lock, [&taskFinish]() { return taskFinish; });

  spdlog::info("clientA finish");
}

void clientB() {
  spdlog::info("clientB start");
  std::mutex _mutex;
  std::condition_variable cv;
  bool taskFinish = false;

  WebRTCApp app;
  WebSocketsClient client;
  std::shared_ptr<PeerConnection> pc;

  OnOpen onOpen = [&client]() {
    // 加入房间
    spdlog::info("clientB WebSockets onOpen");
    // 创建房间
    json joinMessage;
    joinMessage["action"] = "Join";
    joinMessage["rid"] = "rid_test";
    joinMessage["uid"] = "clientB";

    auto result = client.sendMessage(joinMessage.dump());
    if (!result) {
      spdlog::error("clientB WebSockets sendMessage error");
      return;
    }

    spdlog::info("clientB Join Room");
  };

  OnClose onClose = [](const std::string &reason) {
    spdlog::info("clientB WebSockets onClose %s", reason);
  };
  OnError onError = [](const std::string &error) {
    spdlog::error("clientB WebSockets onError %s", error);
  };
  std::function<void(bool)> onJoinResponse = [](bool isOk) {
    if (isOk) {
      spdlog::info("clientB Join Room ok");
    } else {
      spdlog::error("clientB Join Room fail");
    }
  };

  OnMessage onMessage = [onJoinResponse](const std::string &message,
                                         bool isBinary) {
    auto result = json::parse(message);
    spdlog::info("clientB onMessage {}", result["action"]);
    if (result["action"] == "JoinResponse") {
      onJoinResponse(result["ok"]);
      return;
    }
    // todo create answer
  };

  client.onOpen(onOpen);
  client.onClose(onClose);
  client.onError(onError);
  client.onMessage(onMessage);
  client.connect(kServerUrl);

  std::unique_lock<std::mutex> lock(_mutex);
  cv.wait(lock, [&taskFinish]() { return taskFinish; });

  spdlog::info("clientB finish");
}

int main() {
  std::thread t1(clientA);
  // wait clientA create room success
  std::unique_lock<std::mutex> lock(_globalMutex);
  _roomCreatedVar.wait(lock, []() { return _roomCreated; });
  std::thread t2(clientB);

  t1.join();
  t2.join();

  spdlog::info("all clients finish");

  return 0;
}