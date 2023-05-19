#pragma once

#include "sip/inject/interface.hpp"

#include <optional>

#include <Windows.h>

namespace sip::inject {

class Initialization {
public:
  Initialization();

  auto init_window() -> bool;
  auto init_main() -> std::optional<Interface>;

private:  
	HMODULE m_client_dll;
	HMODULE m_steam_api_dll;
  sdk::ISteamUser* m_steam_user;
  
	uintptr_t m_client_size;
	uintptr_t m_steam_api_size;

  auto init_modules() -> bool;
};

} // namespace sip::inject