#include "app.h"
#include "libwebrtc.h"
#include "rtc_types.h"
using namespace libwebrtc;
namespace easywebrtc {

WebRTCApp::WebRTCApp() {
  LibWebRTC::Initialize();
  _factory = LibWebRTC::CreateRTCPeerConnectionFactory();
  _desktop_device = _factory->GetDesktopDevice();
}

WebRTCApp::~WebRTCApp() { LibWebRTC::Terminate(); }

PeerConnection WebRTCApp::createPeerConnection(const RTCConfig &config) {
  RTCConfiguration configuration;
  for (int i = 0; i < libwebrtc::kMaxIceServerSize; i++) {
    configuration.ice_servers[i] = libwebrtc::IceServer{
        config.iceServers[i].uri,
        config.iceServers[i].username,
        config.iceServers[i].password,
    };
  };

  // create media constraints
  scoped_refptr<RTCMediaConstraints> constraints =
      RTCMediaConstraints::Create();

  PeerConnection pc = _factory->Create(configuration, constraints);

  return pc;
}

} // namespace easywebrtc