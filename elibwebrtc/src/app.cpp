#include "app.h"
#include "libwebrtc.h"
using namespace libwebrtc;

WebRTCApp::WebRTCApp() {
  LibWebRTC::Initialize();
  _factory = LibWebRTC::CreateRTCPeerConnectionFactory();
  _desktop_device = _factory->GetDesktopDevice();
}

WebRTCApp::~WebRTCApp() { LibWebRTC::Terminate(); }

void WebRTCApp::Hello() { std::cout << "Hello, world!" << std::endl; }