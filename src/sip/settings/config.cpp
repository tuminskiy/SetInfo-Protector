#include "sip/settings/config.hpp"

#include <fstream>

namespace {
  constexpr std::string_view CONFIG_FIELD = "servers";
} // anonymus namespace


namespace sip::settings {


Config::Config() = default;

Config::Config(Servers servers) 
  : m_servers(std::move(servers))
{ }

auto Config::create_config(const std::filesystem::path& path) -> Config {
  if(!std::filesystem::exists(path)) {
    return {};
  }

  const auto json = nlohmann::json::parse(std::ifstream(path));
  
  return Config{ json.at(CONFIG_FIELD).get<Servers>() };
}


auto Config::get_server_by_address(const Address& address) const -> std::optional<Server> {
  const auto func = [&address](const Server& server) {
    return server.address == address;
  };

  const auto it = std::find_if(std::cbegin(m_servers), std::cend(m_servers), func);

  if (it != std::cend(m_servers))
    return *it;
  
  return std::nullopt;
}


} // namespace sip::settings