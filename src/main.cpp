#include <Windows.h>
#include <thread>
#include <fstream>

#include "sip/init.hpp"
#include "sip/util.hpp"
#include "sdk/steamuser.hpp"


sip::Initialization g_init;
sip::Interface g_handles;
sdk::InitiateGameConnectionFunc g_old_init_game_connect;


auto __fastcall init_game_connection(sdk::ISteamUser *self, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) -> int;

auto __stdcall main_thread(void *hmodule) -> unsigned long;


int __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) {
  DisableThreadLibraryCalls(hmodule);

  switch (reason) {
  case DLL_PROCESS_ATTACH: {
    HANDLE thread_instance =
        CreateThread(nullptr, NULL, main_thread, hmodule, NULL, nullptr);

    if (thread_instance)
      CloseHandle(thread_instance);
    else
      return FALSE;

  } break;
  }

  return TRUE;
}


auto __fastcall init_game_connection(sdk::ISteamUser *self, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure) -> int {

  const auto ip = sip::Util::get_ip_from_node(unIPServer, usPortServer);

  g_handles.engine->pfnConsolePrint(("[SIP] Connected: " + ip + "\n").data());

  return g_old_init_game_connect(self, cbMaxAuthBlob, pData, cbMaxData, steamID, unIPServer, usPortServer, bSecure);
}


auto __stdcall main_thread(void *hmodule) -> unsigned long {
  while (!g_init.init_window()) // wait until the game window is valid
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

  const auto handles = g_init.init_main();

  if (!handles.has_value()) {
    // TODO
    return 0;
  }

  g_handles = handles.value();
  
  // write own InitiateGameConnection function address to virtual table
  g_old_init_game_connect = *(g_handles.init_game_connect);

  DWORD old_protect;
  VirtualProtect(g_handles.init_game_connect, sizeof(void*), PAGE_READWRITE, &old_protect);
  *(g_handles.init_game_connect) = init_game_connection;
  VirtualProtect(g_handles.init_game_connect, sizeof(void*), old_protect, nullptr);

  g_handles.engine->pfnConsolePrint("SetInfoProtecter by Eneu\n");

  return 0;
}