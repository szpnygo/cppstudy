#include "peerconnection.h"
#include "data_channel.h"
#include "rtc_data_channel.h"

namespace easywebrtc {

void PeerConnection::close() { _pc->Close(); }

void PeerConnection::createOffer(OnCreateSuccess success,
                                 OnCreateFailure failure) {
  libwebrtc::scoped_refptr<libwebrtc::RTCMediaConstraints> constraints =
      libwebrtc::RTCMediaConstraints::Create();
  _pc->CreateOffer(
      [success](const libwebrtc::string sdp, const libwebrtc::string type) {
        success(SessionDescription(sdp.std_string(), type.std_string()));
      },
      [failure](const char *error) { failure(error); }, constraints);
}

void PeerConnection::createAnswer(OnCreateSuccess success,
                                  OnCreateFailure failure) {
  libwebrtc::scoped_refptr<libwebrtc::RTCMediaConstraints> constraints =
      libwebrtc::RTCMediaConstraints::Create();
  _pc->CreateAnswer(
      [success](const libwebrtc::string sdp, const libwebrtc::string type) {
        success(SessionDescription(sdp.std_string(), type.std_string()));
      },
      [failure](const char *error) { failure(error); }, constraints);
}

void PeerConnection::setLocalDescription(const SessionDescription &sdp,
                                         OnSetSuccess success,
                                         OnSetFailure failure) {
  _pc->SetLocalDescription(sdp.sdp(), sdp.type(), success,
                           [failure](const char *error) { failure(error); });
}

void PeerConnection::setRemoteDescription(const SessionDescription &sdp,
                                          OnSetSuccess success,
                                          OnSetFailure failure) {
  _pc->SetRemoteDescription(sdp.sdp(), sdp.type(), success,
                            [failure](const char *error) { failure(error); });
}

void PeerConnection::getLocalDescription(OnGetSuccess success,
                                         OnGetFailure failure) {
  _pc->GetLocalDescription(
      [success](const char *sdp, const char *type) {
        success(SessionDescription(sdp, type));
      },
      [failure](const char *error) { failure(error); });
}

void PeerConnection::getRemoteDescription(OnGetSuccess success,
                                          OnGetFailure failure) {
  _pc->GetRemoteDescription(
      [success](const char *sdp, const char *type) {
        success(SessionDescription(sdp, type));
      },
      [failure](const char *error) { failure(error); });
}

std::shared_ptr<DataChannel>
PeerConnection::createDataChannel(const std::string &label) {
  libwebrtc::RTCDataChannelInit *init = new libwebrtc::RTCDataChannelInit();
  libwebrtc::scoped_refptr<libwebrtc::RTCDataChannel> dc =
      _pc->CreateDataChannel(libwebrtc::string(label), init);

  return std::make_shared<DataChannel>(DataChannel(dc));
}

} // namespace easywebrtc