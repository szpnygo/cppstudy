#pragma once
#include "base/scoped_ref_ptr.h"
#include "rtc_data_channel.h"
#include <functional>
#include <iostream>
#include <string>

namespace easywebrtc {

enum DataChannelState {
  DataChannelConnecting,
  DataChannelOpen,
  DataChannelClosing,
  DataChannelClosed,
};

using OnDataChannelState = std::function<void(DataChannelState)>;
using OnDataChannelMessage =
    std::function<void(const char *buffer, int length, bool binary)>;

class ObserverAdapter : public libwebrtc::RTCDataChannelObserver {
public:
  ObserverAdapter(OnDataChannelState onStateChange,
                  OnDataChannelMessage onMessage)
      : _onStateChange(onStateChange), _onMessage(onMessage) {}

  virtual void OnStateChange(libwebrtc::RTCDataChannelState state) override {
    if (_onStateChange) {
      _onStateChange(static_cast<DataChannelState>(state));
    }
  }

  virtual void OnMessage(const char *buffer, int length, bool binary) override {
    if (_onMessage) {
      _onMessage(buffer, length, binary);
    }
  }

private:
  OnDataChannelState _onStateChange;
  OnDataChannelMessage _onMessage;
};

class DataChannel {
public:
  DataChannel(libwebrtc::scoped_refptr<libwebrtc::RTCDataChannel> dc)
      : _dc(dc){};
  ~DataChannel() {}

  void send(const uint8_t *data, uint32_t size, bool binary = false);

  void send(const std::string &data, bool binary = false);

  void send(const std::vector<uint8_t> &data);

  void close();

  const std::string label() const;

  int id() const;

  DataChannelState state();

  void registerObserver(OnDataChannelState onStateChange,
                        OnDataChannelMessage onMessage);

  void unregisterObserver();

private:
  libwebrtc::scoped_refptr<libwebrtc::RTCDataChannel> _dc;
};
}; // namespace easywebrtc
