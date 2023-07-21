#pragma once
#include <istream>
#include <string>

class SessionDescription {
public:
  SessionDescription(std::string sdp, std::string type)
      : _sdp(sdp), _type(type) {}
  ~SessionDescription() {}
  std::string sdp() const { return _sdp; }

  std::string type() const { return _type; }

private:
  std::string _sdp;
  std::string _type;
};