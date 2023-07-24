#pragma once

#include "base/scoped_ref_ptr.h"
#include "candidate.h"
#include "data_channel.h"
#include "rtc_mediaconstraints.h"
#include "rtc_peerconnection.h"
#include "session_description.h"
#include <functional>
#include <iostream>
#include <memory>

namespace easywebrtc {

enum SignalingState {
  SignalingStateStable,
  SignalingStateHaveLocalOffer,
  SignalingStateHaveRemoteOffer,
  SignalingStateHaveLocalPrAnswer,
  SignalingStateHaveRemotePrAnswer,
  SignalingStateClosed
};

enum PeerConnectionState {
  PeerConnectionStateNew,
  PeerConnectionStateConnecting,
  PeerConnectionStateConnected,
  PeerConnectionStateDisconnected,
  PeerConnectionStateFailed,
  PeerConnectionStateClosed,
};

enum IceGatheringState {
  IceGatheringStateNew,
  IceGatheringStateGathering,
  IceGatheringStateComplete
};

enum IceConnectionState {
  IceConnectionStateNew,
  IceConnectionStateChecking,
  IceConnectionStateCompleted,
  IceConnectionStateConnected,
  IceConnectionStateFailed,
  IceConnectionStateDisconnected,
  IceConnectionStateClosed,
  IceConnectionStateMax,
};

typedef std::function<void(SignalingState)> OnSignalingState;
typedef std::function<void(PeerConnectionState)> OnPeerConnectionState;
typedef std::function<void(IceGatheringState)> OnIceGatheringState;
typedef std::function<void(IceConnectionState)> OnIceConnectionState;
typedef std::function<void(const ICECandidate &)> OnIceCandidate;
typedef std::function<void(std::shared_ptr<DataChannel>)> OnDataChannel;

typedef std::function<void(const std::string &)> OnCreateFailure;
typedef std::function<void(const SessionDescription &)> OnCreateSuccess;
typedef std::function<void()> OnSetSuccess;
typedef std::function<void(const std::string &)> OnSetFailure;
typedef std::function<void(const SessionDescription &)> OnGetSuccess;
typedef std::function<void(const std::string &)> OnGetFailure;

class PeerConnection {
public:
  PeerConnection(libwebrtc::scoped_refptr<libwebrtc::RTCPeerConnection> pc)
      : _pc(pc) {
    _observer = new InternalObserver();
    _pc->RegisterRTCPeerConnectionObserver(_observer);
  };

  ~PeerConnection() {}

  void close();

  void createOffer(OnCreateSuccess success, OnCreateFailure failure);
  void createAnswer(OnCreateSuccess success, OnCreateFailure failure);

  void setLocalDescription(const SessionDescription &sdp, OnSetSuccess success,
                           OnSetFailure failure);
  void setRemoteDescription(const SessionDescription &sdp, OnSetSuccess success,
                            OnSetFailure failure);
  void getLocalDescription(OnGetSuccess success, OnGetFailure failure);
  void getRemoteDescription(OnGetSuccess success, OnGetFailure failure);

  void addIceCandidate(const ICECandidate &candidate);

  std::shared_ptr<DataChannel> createDataChannel(const std::string &label);

  // RTCPeerConnectionObserver
  void onSignalingState(OnSignalingState onSignalingState) {
    _observer->onSignalingState = onSignalingState;
  };

  void onPeerConnectionState(OnPeerConnectionState onPeerConnectionState) {
    _observer->onPeerConnectionState = onPeerConnectionState;
  };

  void onIceGatheringState(OnIceGatheringState onIceGatheringState) {
    _observer->onIceGatheringState = onIceGatheringState;
  };

  void onIceConnectionState(OnIceConnectionState onIceConnectionState) {
    _observer->onIceConnectionState = onIceConnectionState;
  };

  void onIceCandidate(OnIceCandidate onIceCandidate) {
    _observer->onIceCandidate = onIceCandidate;
  };

  void onDataChannel(OnDataChannel onDataChannel) {
    _observer->onDataChannel = onDataChannel;
  };

private:
  libwebrtc::scoped_refptr<libwebrtc::RTCPeerConnection> _pc;

  class InternalObserver : public libwebrtc::RTCPeerConnectionObserver {
  public:
    ::easywebrtc::OnSignalingState onSignalingState;
    ::easywebrtc::OnPeerConnectionState onPeerConnectionState;
    ::easywebrtc::OnIceGatheringState onIceGatheringState;
    ::easywebrtc::OnIceConnectionState onIceConnectionState;
    ::easywebrtc::OnIceCandidate onIceCandidate;
    ::easywebrtc::OnDataChannel onDataChannel;

    virtual void OnSignalingState(libwebrtc::RTCSignalingState state) override {
      if (onSignalingState) {
        onSignalingState(static_cast<SignalingState>(state));
      }
    };

    virtual void
    OnPeerConnectionState(libwebrtc::RTCPeerConnectionState state) override {
      if (onPeerConnectionState) {
        onPeerConnectionState(static_cast<PeerConnectionState>(state));
      }
    };

    virtual void
    OnIceGatheringState(libwebrtc::RTCIceGatheringState state) override {
      if (onIceGatheringState) {
        onIceGatheringState(static_cast<IceGatheringState>(state));
      }
    };

    virtual void
    OnIceConnectionState(libwebrtc::RTCIceConnectionState state) override {
      if (onIceConnectionState) {
        onIceConnectionState(static_cast<IceConnectionState>(state));
      }
    };

    virtual void OnIceCandidate(
        libwebrtc::scoped_refptr<libwebrtc::RTCIceCandidate> candidate)
        override {
      if (onIceCandidate) {
        onIceCandidate(ICECandidate(candidate));
      }
    };

    virtual void OnAddStream(
        libwebrtc::scoped_refptr<libwebrtc::RTCMediaStream> stream) override{};

    virtual void OnRemoveStream(
        libwebrtc::scoped_refptr<libwebrtc::RTCMediaStream> stream) override{};

    virtual void OnDataChannel(
        libwebrtc::scoped_refptr<libwebrtc::RTCDataChannel> data_channel)
        override {
      if (onDataChannel) {
        onDataChannel(std::make_shared<DataChannel>(data_channel));
      }
    };

    virtual void OnRenegotiationNeeded() override{};

    virtual void OnTrack(libwebrtc::scoped_refptr<libwebrtc::RTCRtpTransceiver>
                             transceiver) override{};

    virtual void OnAddTrack(
        libwebrtc::vector<libwebrtc::scoped_refptr<libwebrtc::RTCMediaStream>>
            streams,
        libwebrtc::scoped_refptr<libwebrtc::RTCRtpReceiver> receiver)
        override{};

    virtual void
    OnRemoveTrack(libwebrtc::scoped_refptr<libwebrtc::RTCRtpReceiver> receiver)
        override{};
  };

  InternalObserver *_observer;
};
} // namespace easywebrtc