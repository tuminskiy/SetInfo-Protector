#pragma once

#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

namespace sip::settings {

struct Address {
  std::string ip;
  std::uint16_t port;
};

auto operator== (const Address& lhs, const Address& rhs) -> bool;

auto to_string(const Address& address) -> std::string;

} // namespace sip::settings

namespace {
  
constexpr std::string_view ADDRESS_IP_FIELD = "ip";
constexpr std::string_view ADDRESS_PORT_FIELD = "port";

} // anonymous namespace


namespace nlohmann {

template <>
struct adl_serializer<sip::settings::Address> {

  template <class BasicJsonType>
  static auto from_json(const BasicJsonType& j, sip::settings::Address& address) -> void {
    j.at(ADDRESS_IP_FIELD).get_to(address.ip);
    j.at(ADDRESS_PORT_FIELD).get_to(address.port);
  }

  template <class BasicJsonType>
  static auto to_json(BasicJsonType& j, const sip::settings::Address& address) -> void {
    j[ADDRESS_IP_FIELD] = address.ip;
    j[ADDRESS_PORT_FIELD] = address.port;
  };

};

} // namespace nlohmann