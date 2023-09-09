#include "sip/settings/server.hpp"
#include "sip/settings/address.hpp"

#include <vector>
#include <string_view>
#include <filesystem>
#include <optional>

#include <nlohmann/json.hpp>

namespace sip::settings {


class Config {
  using Servers = std::vector<Server>;

  Servers m_servers;

public:
  Config();
  Config(Servers servers);

  static auto create_config(const std::filesystem::path& path) -> Config;

  auto get_server_by_address(const Address& address) const -> std::optional<Server>;

  auto get_servers() const -> const Servers&;
};




} // namespace sip::settings

