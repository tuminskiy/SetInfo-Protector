#include <Windows.h>
#include <thread>
#include "sip/inject/init.hpp"
#include "sip/inject/util.hpp"
#include "sip/settings/config.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

sip::inject::Initialization g_init;
sip::inject::Interface g_handles;
sip::settings::Config g_config;

sdk::InitiateGameConnectionFunc g_old_init_game_connect;
sdk::TerminateGameConnectionFunc g_old_term_game_connect;

auto __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) -> int;


auto __fastcall init_game_connection(void* pAuthBlob, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) -> int;

auto __fastcall terminate_game_connection(void* pAuthBlob, int cbMaxAuthBlob, uint32_t unIPServer, uint16_t usPortServer) -> void;


auto overwrite_init_game_connection() -> void;

auto overwrite_term_game_connection() -> void;


auto __stdcall main_thread(void *hmodule) -> unsigned long {
  while (!g_init.init_window()) // wait until the game window is valid
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  
  const auto handles = g_init.init_main();

  if (!handles.has_value()) {
    // TODO
    return 0;
  }

  g_handles = handles.value();
  
  overwrite_init_game_connection();
  overwrite_term_game_connection();

  g_handles.console_print("SetInfoProtecter by Eneu\n");

  try
  {
    g_config = sip::settings::parse_config("test.json");
  } catch (const std::exception& e) {
    g_handles.console_print("Failed parse SIP config:\n");
    g_handles.console_print(e.what());
    g_handles.console_print("\n");
  }

  return 0;
}


auto __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) -> int {
  DisableThreadLibraryCalls(hmodule);

  switch (reason) {
    case DLL_PROCESS_ATTACH: {
      // dont crash, but doesnt work :)
      // std::thread t([] { main_thread(nullptr); });
      // t.detach();

      HANDLE thread_instance = CreateThread(nullptr, NULL, main_thread, hmodule, NULL, nullptr);

      if (thread_instance)
        CloseHandle(thread_instance);
      else
        return TRUE;

    } break;
  }

  return TRUE;
}


auto __fastcall init_game_connection(void* pAuthBlob, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) -> int {

  const auto address = sip::settings::get_address_from_node(unIPServer, usPortServer);

  const auto infos = sip::settings::get_info_by_address(g_config, address);

  for (const auto& info : infos) {
    g_handles.set_info(info.key, info.value);
  }

  return g_old_init_game_connect(pAuthBlob, cbMaxAuthBlob, pData, cbMaxData, steamID, unIPServer, usPortServer, bSecure);
}

auto __fastcall terminate_game_connection(void* pAuthBlob, int cbMaxAuthBlob, uint32_t unIPServer, uint16_t usPortServer) -> void {
  const auto address = sip::settings::get_address_from_node(unIPServer, usPortServer);
  const auto infos = sip::settings::get_info_by_address(g_config, address);

  for (const auto& info : infos) {
    g_handles.set_info(info.key, "");
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