#pragma once

#include "rtc_ice_candidate.h"
#include "rtc_types.h"
#include <cstddef>
#include <string>

namespace easywebrtc {
class ICECandidate {
public:
  ICECandidate(libwebrtc::scoped_refptr<libwebrtc::RTCIceCandidate> candidate)
      : _candidate(candidate){};

  ICECandidate(const std::string &candidate, const std::string &sdp_mid,
               int sdp_mline_index) {
    libwebrtc::SdpParseError *err = new libwebrtc::SdpParseError();
    _candidate = libwebrtc::RTCIceCandidate::Create(candidate, sdp_mid,
                                                    sdp_mline_index, err);
  };

  const std::string candidate() const {
    return _candidate->candidate().std_string();
  }

  const std::string sdp_mid() const {
    return _candidate->sdp_mid().std_string();
  }

  int sdp_mline_index() const { return _candidate->sdp_mline_index(); }

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