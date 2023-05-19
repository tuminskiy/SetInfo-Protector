#pragma once

#include "sip/settings/info.hpp"
#include "sip/settings/address.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace sip::settings {

struct Server {
  Address address;
  std::vector<Info> infos;
  bool filterstruffcmd = true;
};

} // namespace sip::settings

namespace {

constexpr std::string_view SERVER_ADDRESS_FIELD = "address";
constexpr std::string_view SERVER_INFOS_FIELD = "infos";
constexpr std::string_view SERVER_FILTERSTUFFCMD = "filterstuffcmd";

} // anonymous namespace

namespace nlohmann {

template <>
struct adl_serializer<sip::settings::Server> {

  template <class BasicJsonType>
  static auto from_json(const BasicJsonType& j, sip::settings::Server& server) -> void {
    j.at(SERVER_ADDRESS_FIELD).get_to(server.address);
    j.at(SERVER_INFOS_FIELD).get_to(server.infos);
    
    if (j.count(SERVER_FILTERSTUFFCMD) != 0) {
      j.at(SERVER_FILTERSTUFFCMD).get_to(server.filterstruffcmd);
    }
  }
  
  template <class BasicJsonType>
  static auto to_json(BasicJsonType& j, const sip::settings::Server& server) -> void {
    j[SERVER_ADDRESS_FIELD] = server.address;
    j[SERVER_INFOS_FIELD] = server.infos;
    j[SERVER_FILTERSTUFFCMD] = server.filterstruffcmd;
  }
};

} // namespace nlohmann