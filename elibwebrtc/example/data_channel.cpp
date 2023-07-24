#include "data_channel.h"
#include "app.h"
#include "candidate.h"
#include "peerconnection.h"
#include "session_description.h"
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

std::string candidateToJSON(const ICECandidate &candidate, std::string uid,
                            std::string from) {
  json candidateData;
  candidateData["candidate"] = candidate.candidate();
  candidateData["sdp_mid"] = candidate.sdp_mid();
  candidateData["sdp_mline_index"] = candidate.sdp_mline_index();

  json candidateMessage;
  candidateMessage["action"] = "SendRTCData";
  candidateMessage["rid"] = "rid_test";
  candidateMessage["uid"] = uid;
  candidateMessage["from"] = from;
  candidateMessage["type"] = "candidate";
  candidateMessage["data"] = candidateData.dump();

  return candidateMessage.dump();
}

void clientA() {
  spdlog::info("clientA start");
  std::mutex _mutex;
  std::condition_variable cv;
  bool taskFinish = false;

  WebRTCApp app;
  WebSocketsClient client;
  std::shared_ptr<PeerConnection> pc;
  std::shared_ptr<DataChannel> dc;

  std::function<void()> dataChannel = [&dc]() {
    dc->send("I'm ClientA");
    spdlog::info("clientA send message");
  };

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

  std::function<void()> onJoinNotify = [&app, &pc, &dc, &client, dataChannel,
                                        &_mutex, &taskFinish, &cv]() {
    spdlog::info("clientA Join Room Notify");
    spdlog::info("<--------- start WebRTC --------->");
    // 创建offer
    pc = app.createPeerConnection({});
    pc->onIceCandidate([&client](ICECandidate candidate) {
      spdlog::info("clientA onIceCandidate");
      bool result =
          client.sendMessage(candidateToJSON(candidate, "clientB", "clientA"));
      if (result) {
        spdlog::info("clientA Send Candidate success");
      } else {
        spdlog::error("clientA Send Candidate error");
      }
    });
    pc->onPeerConnectionState([dataChannel](PeerConnectionState state) {
      if (state == PeerConnectionState::PeerConnectionStateConnected) {
        spdlog::info("<--------- clientA Connected --------->");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        dataChannel();
      }
    });

    dc = pc->createDataChannel("test");
    OnDataChannelMessage onMessage = [&_mutex, &taskFinish,
                                      &cv](const char *buffer, int length,
                                           bool binary) {
      std::string data(buffer, length);
      spdlog::info("clientA DataChannelMessage => {} <= ", data);
      // end task
      std::unique_lock<std::mutex> lock(_mutex);
      taskFinish = true;
      cv.notify_all();
    };
    OnDataChannelState onState = [](DataChannelState state) {};
    dc->registerObserver(onState, onMessage);

    pc->createOffer(
        [&pc, &client](const SessionDescription &offer) {
          spdlog::info("clientA createOffer success ");
          std::string sdp = offer.sdp();
          pc->setLocalDescription(
              offer,
              [sdp, &client]() {
                spdlog::info("clientA setLocalDescription success");

                // 发送offer给clientB
                json offerMessage;
                offerMessage["action"] = "SendRTCData";
                offerMessage["rid"] = "rid_test";
                offerMessage["uid"] = "clientB";
                offerMessage["from"] = "clientA";
                offerMessage["type"] = "offer";
                offerMessage["data"] = sdp;

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
  };

  OnMessage onMessage = [onCreateRemoteRoomResponse, onJoinNotify,
                         &pc](const std::string &message, bool isBinary) {
    auto result = json::parse(message);
    // spdlog::info("clientA onMessage {}", result["action"]);
    if (result["action"] == "CreateRemoteRoomResponse") {
      onCreateRemoteRoomResponse(result["ok"]);
      return;
    } else if (result["action"] == "JoinNotify") {
      spdlog::info("clientA JoinNotify from {}", result["uid"]);
      onJoinNotify();
      return;
    } else if (result["action"] == "SendRTCData") {
      if (result["type"] == "answer") {
        SessionDescription answer(result["data"], "answer");
        pc->setRemoteDescription(
            answer,
            []() { spdlog::info("clientA setRemoteDescription success"); },
            [](const std::string &error) {
              spdlog::error("clientA setRemoteDescription error %s", error);
            });
      } else if (result["type"] == "candidate") {
        auto candidate = json::parse(std::string(result["data"]));
        ICECandidate iceCandidate(candidate["candidate"], candidate["sdp_mid"],
                                  candidate["sdp_mline_index"]);
        pc->addIceCandidate(iceCandidate);
        spdlog::info("clientA addIceCandidate success");
      }
    }
  };

  client.onOpen(onOpen);
  client.onMessage(onMessage);
  client.connect(kServerUrl);

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
  std::shared_ptr<DataChannel> dc;

  std::function<void()> dataChannel = [&dc, &_mutex, &taskFinish, &cv]() {
    spdlog::info("clientB DataChannel");
    OnDataChannelMessage onMessage = [&dc, &_mutex, &taskFinish,
                                      &cv](const char *buffer, int length,
                                           bool binary) {
      std::string data(buffer, length);
      spdlog::info("clientB DataChannelMessage => {} <= ", data);
      dc->send("I'm ClientB");

      // end task
      std::unique_lock<std::mutex> lock(_mutex);
      taskFinish = true;
      cv.notify_all();
    };
    OnDataChannelState onState = [](DataChannelState state) {};
    dc->registerObserver(onState, onMessage);
  };

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

  std::function<void(bool)> onJoinResponse = [](bool isOk) {
    if (isOk) {
      spdlog::info("clientB Join Room ok");
    } else {
      spdlog::error("clientB Join Room fail");
    }
  };

  std::function<void(json)> onOffer = [&app, &pc, &client, &dc,
                                       dataChannel](json result) {
    pc = app.createPeerConnection({});
    pc->onIceCandidate([&client](ICECandidate candidate) {
      spdlog::info("clientB onIceCandidate");
      bool result =
          client.sendMessage(candidateToJSON(candidate, "clientA", "clientB"));
      if (result) {
        spdlog::info("clientB Send Candidate success");
      } else {
        spdlog::error("clientB Send Candidate error");
      }
    });

    pc->onDataChannel([&dc, dataChannel](std::shared_ptr<DataChannel> d) {
      spdlog::info("clientB onDataChannel");
      dc = d;
      dataChannel();
    });

    pc->onPeerConnectionState([](PeerConnectionState state) {
      if (state == PeerConnectionState::PeerConnectionStateConnected) {
        spdlog::info("<--------- clientB Connected --------->");
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });

    SessionDescription offer(result["data"], "offer");
    pc->setRemoteDescription(
        offer,
        [&pc, &client]() {
          spdlog::info("clientB setRemoteDescription success");
          pc->createAnswer(
              [&pc, &client](const SessionDescription &answer) {
                spdlog::info("clientB createAnswer success");
                std::string sdp = answer.sdp();
                pc->setLocalDescription(
                    answer,
                    [sdp, &client]() {
                      spdlog::info("clientB setLocalDescription success");

                      // 发送offer给clientB
                      json answerMessage;
                      answerMessage["action"] = "SendRTCData";
                      answerMessage["rid"] = "rid_test";
                      answerMessage["uid"] = "clientA";
                      answerMessage["from"] = "clientB";
                      answerMessage["type"] = "answer";
                      answerMessage["data"] = sdp;

                      auto result = client.sendMessage(answerMessage.dump());
                      if (result) {
                        spdlog::info("clientB Send Answer success");
                      } else {
                        spdlog::error("clientB Send Answer error");
                      }
                    },
                    [](const std::string &error) {
                      spdlog::error("clientB setLocalDescription error %s",
                                    error);
                    });
              },
              [](const std::string &error) {
                spdlog::error("clientB createAnswer error %s", error);
              });
        },
        [](const std::string &error) {
          spdlog::error("clientB setRemoteDescription error %s", error);
        });
  };

  OnMessage onMessage = [onJoinResponse, onOffer,
                         &pc](const std::string &message, bool isBinary) {
    auto result = json::parse(message);
    // spdlog::info("clientB onMessage {}", result["action"]);
    if (result["action"] == "JoinResponse") {
      onJoinResponse(result["ok"]);
      return;
    } else if (result["action"] == "SendRTCData") {
      if (result["type"] == "candidate") {
        auto candidate = json::parse(std::string(result["data"]));
        ICECandidate iceCandidate(candidate["candidate"], candidate["sdp_mid"],
                                  candidate["sdp_mline_index"]);
        pc->addIceCandidate(iceCandidate);
        spdlog::info("clientB addIceCandidate success");
      } else if (result["type"] == "offer") {
        onOffer(result);
      }
    }
  };

  client.onOpen(onOpen);
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