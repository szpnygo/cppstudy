#pragma once

#include "base/fixed_size_function.h"
#include "base/scoped_ref_ptr.h"
#include "rtc_mediaconstraints.h"
#include "rtc_peerconnection.h"
#include "session_description.h"
#include <functional>

namespace easywebrtc {

typedef std::function<void(const std::string &)> OnCreateFailure;
typedef std::function<void(const SessionDescription &)> OnCreateSuccess;
typedef std::function<void()> OnSetSuccess;
typedef std::function<void(const std::string &)> OnSetFailure;
typedef std::function<void(const SessionDescription &)> OnGetSuccess;
typedef std::function<void(const std::string &)> OnGetFailure;

class PeerConnection {
public:
  PeerConnection(libwebrtc::scoped_refptr<libwebrtc::RTCPeerConnection> pc)
      : _pc(pc){};

  ~PeerConnection() {}

  void createOffer(OnCreateSuccess success, OnCreateFailure failure);
  void createAnswer(OnCreateSuccess success, OnCreateFailure failure);
  void setLocalDescription(const SessionDescription &sdp, OnSetSuccess success,
                           OnSetFailure failure);
  void setRemoteDescription(const SessionDescription &sdp, OnSetSuccess success,
                            OnSetFailure failure);
  void getLocalDescription(OnGetSuccess success, OnGetFailure failure);
  void getRemoteDescription(OnGetSuccess success, OnGetFailure failure);

private:
  libwebrtc::scoped_refptr<libwebrtc::RTCPeerConnection> _pc;
};
} // namespace easywebrtc