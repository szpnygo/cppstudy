#pragma once

#include "base/scoped_ref_ptr.h"
#include "rtc_peerconnection.h"

namespace easywebrtc {

class PeerConnection {
public:
  PeerConnection(libwebrtc::scoped_refptr<libwebrtc::RTCPeerConnection> pc)
      : _pc(pc){};

  ~PeerConnection() {}

private:
  libwebrtc::scoped_refptr<libwebrtc::RTCPeerConnection> _pc;
};
} // namespace easywebrtc