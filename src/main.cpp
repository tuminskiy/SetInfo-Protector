#include "sip/inject/init.hpp"
#include "sip/inject/util.hpp"
#include "sip/settings/config.hpp"

#include <thread>
#include <fstream>

#include <nlohmann/json.hpp>

#include <Windows.h>


sip::inject::Interface g_handles;
sip::settings::Config g_config;

sdk::InitiateGameConnectionFunc g_old_init_game_connect;
sdk::TerminateGameConnectionFunc g_old_term_game_connect;

std::ofstream fout_log("sip.log", std::ios::app);

auto __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) -> int;


auto __fastcall init_game_connection(void* pAuthBlob, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) -> int;

auto __fastcall terminate_game_connection(void* pAuthBlob, int cbMaxAuthBlob, uint32_t unIPServer, uint16_t usPortServer) -> void;


auto overwrite_init_game_connection() -> void;

auto overwrite_term_game_connection() -> void;

auto add_reload_config_cmd() -> void;

auto load_config() -> void;


auto __stdcall main_thread(void *hmodule) -> unsigned long {
  sip::inject::Initialization init;

  while (!init.init_window()) // wait until the game window is valid
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

  const auto handles = init.init_main();

  if (!handles.has_value()) {
    fout_log << "[" << sip::inject::util::get_current_time() << "] "
             << "Failed handle modules" << std::endl;
    return 0;
  }

  g_handles = handles.value();
  
  overwrite_init_game_connection();
  overwrite_term_game_connection();

  add_reload_config_cmd();

  load_config();

  return 0;
}


auto __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) -> int {
  DisableThreadLibraryCalls(hmodule);

  switch (reason) {
    case DLL_PROCESS_ATTACH: {
      CreateThread(nullptr, NULL, main_thread, hmodule, 0, nullptr);
    } break;
  }

  return TRUE;
}

extern "C" __declspec(dllexport)
auto  __stdcall RIB_Main(LPVOID lp, LPVOID lp2, LPVOID lp3, LPVOID lp4, LPVOID lp5 ) -> BOOL {
  return TRUE;
}


auto __fastcall init_game_connection(void* pAuthBlob, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) -> int {

  const auto address = sip::inject::util::get_address_from_node(unIPServer, usPortServer);

  g_handles.console_print("[SIP] Connecting to %s\n", sip::settings::to_string(address).data());

  const auto server = g_config.get_server_by_address(address);

  if (server.has_value()) {
    const auto infos = server.value().infos;

    for (const auto& info : infos) {
      g_handles.set_info(info.key, info.value);
    }
    
    g_handles.set_filterstuffcmd(server.value().filterstruffcmd);
  }

  return g_old_init_game_connect(pAuthBlob, cbMaxAuthBlob, pData, cbMaxData, steamID, unIPServer, usPortServer, bSecure);
}

auto __fastcall terminate_game_connection(void* pAuthBlob, int cbMaxAuthBlob, uint32_t unIPServer, uint16_t usPortServer) -> void {
  const auto address = sip::inject::util::get_address_from_node(unIPServer, usPortServer);

  g_handles.console_print("[SIP] Disconnecting from %s\n", sip::settings::to_string(address).data());

  const auto server = g_config.get_server_by_address(address);

  if (server.has_value()) {
    const auto infos = server.value().infos;

    for (const auto& info : infos) {
      g_handles.set_info(info.key, "");
    }

    g_handles.set_filterstuffcmd(true);
  }
  

  g_old_term_game_connect(pAuthBlob, cbMaxAuthBlob, unIPServer, usPortServer);
}


auto overwrite_init_game_connection() -> void {
  g_old_init_game_connect = *(g_handles.init_game_connect);

  DWORD old_protect;
  VirtualProtect(g_handles.init_game_connect, sizeof(void*), PAGE_READWRITE, &old_protect);
  *(g_handles.init_game_connect) = init_game_connection;
  VirtualProtect(g_handles.init_game_connect, sizeof(void*), old_protect, nullptr);
}

auto overwrite_term_game_connection() -> void {
  g_old_term_game_connect = *(g_handles.term_game_connect);

  DWORD old_protect;
  VirtualProtect(g_handles.term_game_connect, sizeof(void*), PAGE_READWRITE, &old_protect);
  *(g_handles.term_game_connect) = terminate_game_connection;
  VirtualProtect(g_handles.term_game_connect, sizeof(void*), old_protect, nullptr);
}


auto add_reload_config_cmd() -> void {
  g_handles.engine->pfnAddCommand("sip_reload_cfg", load_config);
}


auto load_config() -> void {
  try
  {
    g_config = sip::settings::Config::create_config("sip.json");
    g_handles.console_print("[SIP] Config reloaded\n");

  } catch (const std::exception& e) {
    fout_log << "[" << sip::inject::util::get_current_time() << "] "
             << "Parse config exception:\n"
             << e.what() << std::endl;

    g_handles.console_print("[SIP] Config parsing error. See sip.log for details\n");
  }
}