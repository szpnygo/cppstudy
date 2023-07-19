#include "app.h"
#include "libwebrtc.h"
using namespace libwebrtc;
namespace easywebrtc {

WebRTCApp::WebRTCApp() {
  LibWebRTC::Initialize();
  _factory = LibWebRTC::CreateRTCPeerConnectionFactory();
  _desktop_device = _factory->GetDesktopDevice();
}

WebRTCApp::~WebRTCApp() { LibWebRTC::Terminate(); }

void WebRTCApp::createPeerConnection() {}

bool WebRTCApp::parseConfiguration() { return true; }

} // namespace easywebrtc