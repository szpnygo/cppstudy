
#pragma once
#include "base/scoped_ref_ptr.h"
#include "libwebrtc.h"
#include "rtc_audio_device.h"
#include "rtc_desktop_device.h"
#include "rtc_dtmf_sender.h"
#include "rtc_media_stream.h"
#include "rtc_media_track.h"
#include "rtc_mediaconstraints.h"
#include "rtc_peerconnection.h"
#include "rtc_peerconnection_factory.h"
#include "rtc_video_device.h"
#include <iostream>

using namespace libwebrtc;

class WebRTCApp {
public:
  WebRTCApp();
  ~WebRTCApp();

  void Hello();

private:
  scoped_refptr<RTCPeerConnectionFactory> _factory;
  scoped_refptr<RTCDesktopDevice> _desktop_device;
};