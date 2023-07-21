
#pragma once
#include "base/scoped_ref_ptr.h"
#include "libwebrtc.h"
#include "peerconnection.h"
#include "rtc_audio_device.h"
#include "rtc_data_channel.h"
#include "rtc_desktop_device.h"
#include "rtc_dtmf_sender.h"
#include "rtc_media_stream.h"
#include "rtc_media_track.h"
#include "rtc_mediaconstraints.h"
#include "rtc_peerconnection.h"
#include "rtc_peerconnection_factory.h"
#include "rtc_video_device.h"
#include "types.h"
#include <iostream>
#include <memory>
#include <unordered_map>

namespace easywebrtc {

using namespace libwebrtc;

using DataChannel = scoped_refptr<RTCDataChannel>;

class WebRTCApp {
public:
  WebRTCApp();
  ~WebRTCApp();

  std::shared_ptr<PeerConnection> createPeerConnection(const RTCConfig &config);

private:
  // peer connection factory
  scoped_refptr<RTCPeerConnectionFactory> _factory;
  // manage all peer connections
  std::unordered_map<std::string, PeerConnection> _peerconnections;

  // desktop device
  scoped_refptr<RTCDesktopDevice> _desktop_device;
};

} // namespace easywebrtc