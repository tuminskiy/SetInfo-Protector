#include "sip/settings/address.hpp"

namespace sip::settings {

auto operator== (const Address& lhs, const Address& rhs) -> bool {
  return lhs.ip == rhs.ip
      && lhs.port == rhs.port;
}

auto to_string(const Address& address) -> std::string {
  return address.ip + ":" + std::to_string(address.port);
}

} // namespace sip::settings