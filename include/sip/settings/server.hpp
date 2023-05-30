#pragma once

#include "sip/settings/info.hpp"
#include "sip/settings/address.hpp"
#include "sip/settings/cvar.hpp"
#include "sip/inject/util.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <nlohmann/json.hpp>

namespace sip::settings {


struct Server {
  std::string name;
  Address address;
  std::vector<Info> infos;
  std::vector<Cvar> cvars;
};


} // namespace sip::settings



namespace nlohmann {


template <>
struct adl_serializer<std::vector<sip::settings::Info>> {

  template <class BasicJsonType>
  static auto from_json(const BasicJsonType& j, std::vector<sip::settings::Info>& infos) -> void {
    infos.reserve(j.size());

    for (const auto& info : j.items()) {
      infos.push_back({ info.key(), info.value() });
    }
  }
  
  template <class BasicJsonType>
  static auto to_json(BasicJsonType& j, const std::vector<sip::settings::Info>& infos) -> void {
    for (const auto& info : infos) {
      j[info.key] = info.value;
    }
  }

};


template <>
struct adl_serializer<sip::settings::Server> {

  static constexpr std::string_view SERVER_NAME_FIELD = "name";
  static constexpr std::string_view SERVER_ADDRESS_FIELD = "address";
  static constexpr std::string_view SERVER_INFOS_FIELD = "infos";
  static constexpr std::string_view SERVER_CVAR_FIELD = "cvars";

  template <class BasicJsonType>
  static auto from_json(const BasicJsonType& j, sip::settings::Server& server) -> void {
    j.at(SERVER_NAME_FIELD).get_to(server.name);
    j.at(SERVER_ADDRESS_FIELD).get_to(server.address);

    sip::inject::util::get_value_or_default(j, SERVER_INFOS_FIELD, &server.infos);
    sip::inject::util::get_value_or_default(j, SERVER_CVAR_FIELD, &server.cvars);
  }
  
  template <class BasicJsonType>
  static auto to_json(BasicJsonType& j, const sip::settings::Server& server) -> void {
    j[SERVER_NAME_FIELD] = server.name;
    j[SERVER_ADDRESS_FIELD] = server.address;
    j[SERVER_INFOS_FIELD] = server.infos;
    j[SERVER_CVAR_FIELD] = server.cvars;
  }

};

} // namespace nlohmann