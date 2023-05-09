#include "sip/settings/config.hpp"

#include <fstream>

namespace sip::settings {


constexpr std::string_view CONFIG_FIELD = "servers";

auto parse_config(const std::filesystem::path& path) -> Config {
  if(!std::filesystem::exists(path)) {
    return {};
  }

  const auto json = nlohmann::json::parse(std::ifstream(path));
  
  return json.at(CONFIG_FIELD).get<Config>();
}

auto get_info_by_address(const Config& config, const Address& address) -> std::vector<Info> {
  const auto func = [&address](const Server& server) {
    return server.address == address;
  };

  const auto it = std::find_if(std::cbegin(config), std::cend(config), func);

  return it != std::cend(config) ? it->infos : std::vector<Info>{};
}

auto get_address_from_node(uint32_t unIPServer, std::uint16_t usPortServer) -> Address
{
  constexpr uint32_t node_a_mask = 0b1111'1111'0000'0000'0000'0000'0000'0000;
  constexpr uint32_t node_b_mask = 0b0000'0000'1111'1111'0000'0000'0000'0000;
  constexpr uint32_t node_c_mask = 0b0000'0000'0000'0000'1111'1111'0000'0000;
  constexpr uint32_t node_d_mask = 0b0000'0000'0000'0000'0000'0000'1111'1111;

  const uint8_t node_a = (unIPServer & node_a_mask) >> 24;
  const uint8_t node_b = (unIPServer & node_b_mask) >> 16;
  const uint8_t node_c = (unIPServer & node_c_mask) >> 8;
  const uint8_t node_d = (unIPServer & node_d_mask);

  Address address;
  address.ip = std::to_string(node_a) + "." + std::to_string(node_b) + "." + std::to_string(node_c) + "." + std::to_string(node_d);
  address.port = usPortServer;

  return address;
}


} // namespace sip::settings