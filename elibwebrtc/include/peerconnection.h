#include "app.h"
#include "rtc_types.h"
namespace easywebrtc {

class PeerConnection {
public:
  PeerConnection(WebRTCApp *app) : _app(app) {}
  ~PeerConnection() {}

private:
  WebRTCApp *_app;
};
} // namespace easywebrtc