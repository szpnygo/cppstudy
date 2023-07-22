#include "data_channel.h"

namespace easywebrtc {

void DataChannel::close() { _dc->Close(); };

const std::string DataChannel::label() const {
  return _dc->label().std_string();
};

int DataChannel::id() const { return _dc->id(); };

DataChannelState DataChannel::state() {
  return static_cast<DataChannelState>(_dc->state());
};

void DataChannel::send(const uint8_t *data, uint32_t size, bool binary) {
  _dc->Send(data, size, binary);
};

void DataChannel::send(const std::string &data, bool binary) {
  send(reinterpret_cast<const uint8_t *>(data.c_str()), data.size(), binary);
};

void DataChannel::send(const std::vector<uint8_t> &data) {
  send(data.data(), data.size(), true);
};

void DataChannel::registerObserver(OnDataChannelState onStateChange,
                                   OnDataChannelMessage onMessage) {
  ObserverAdapter *adapter = new ObserverAdapter(onStateChange, onMessage);
  _dc->RegisterObserver(adapter);
}

void DataChannel::unregisterObserver() { _dc->UnregisterObserver(); }

} // namespace easywebrtc