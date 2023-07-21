#pragma once
#include "rtc_types.h"
#include <iostream>

struct IceServer {
  std::string uri;
  std::string username;
  std::string password;
};

struct RTCConfig {
  IceServer iceServers[libwebrtc::kMaxIceServerSize];
};
