#pragma once

#include "sip/inject/sighelper.hpp"
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
  //void* m_game_window;
  int m_game_pid;
  
  HMODULE m_hardware_dll;
	HMODULE m_client_dll;
	HMODULE m_steam_api_dll;
  sdk::ISteamUser* m_steam_user;
  
	uintptr_t m_hardware_size;
	uintptr_t m_client_size;
	uintptr_t m_steam_api_size;

  SigHelper m_engine_sig;

  auto init_modules() -> bool;
};

} // namespace sip::inject