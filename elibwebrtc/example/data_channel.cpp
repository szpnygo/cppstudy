#include "app.h"
#include "spdlog/spdlog.h"
#include "websockets.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace easywebrtc;

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

  std::function<void()> onJoinNotify = [&app, &pc]() {
    spdlog::info("clientB Join Room Notify, start create offer");
    // 创建offer
    pc = app.createPeerConnection({});
  };

  OnMessage onMessage = [onCreateRemoteRoomResponse, onJoinNotify](
                            const std::string &message, bool isBinary) {
    auto result = json::parse(message);
    if (result["action"] == "CreateRemoteRoomResponse") {
      onCreateRemoteRoomResponse(result["ok"]);
      return;
    } else if (result["action"] == "JoinNotify") {
      onJoinNotify();
      return;
    }
  };

  client.onOpen(onOpen);
  client.onClose(onClose);
  client.onError(onError);
  client.onMessage(onMessage);
  client.connect("wss://api.desktop.test.tcode.ltd/base");

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
      // 创建房间成功
      std::unique_lock<std::mutex> lock(_globalMutex);
      _roomCreated = true;
      _roomCreatedVar.notify_one();
    } else {
      spdlog::error("clientB Join Room fail");
    }
  };

  OnMessage onMessage = [onJoinResponse](const std::string &message,
                                         bool isBinary) {
    auto result = json::parse(message);
    if (result["action"] == "JoinResponse") {
      onJoinResponse(result["ok"]);
      return;
    }
  };

  client.onOpen(onOpen);
  client.onClose(onClose);
  client.onError(onError);
  client.onMessage(onMessage);
  client.connect("wss://api.desktop.test.tcode.ltd/base");

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