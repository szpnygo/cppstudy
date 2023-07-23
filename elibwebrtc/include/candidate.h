#pragma once

#include "rtc_ice_candidate.h"
#include <string>

namespace easywebrtc {
class ICECandidate {
public:
  ICECandidate(libwebrtc::scoped_refptr<libwebrtc::RTCIceCandidate> candidate)
      : _candidate(candidate){};

  const std::string candidate() { return _candidate->candidate().std_string(); }

  const std::string sdp_mid() { return _candidate->sdp_mid().std_string(); }

  int sdp_mline_index() { return _candidate->sdp_mline_index(); }

  bool ToString(std::string &out) {
    libwebrtc::string s;
    auto result = _candidate->ToString(s);
    out = s.std_string();
    return result;
  }

private:
  libwebrtc::scoped_refptr<libwebrtc::RTCIceCandidate> _candidate;
};
}; // namespace easywebrtc