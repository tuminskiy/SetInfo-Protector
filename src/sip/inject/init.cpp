#include "sip/inject/init.hpp"
#include "sip/inject/const.hpp"
#include "sip/inject/util.hpp"

#include <thread>

#include <process.h>

#include <fstream>
#include <iomanip>

sdk::cl_enginefunc_t* gEngine;

namespace sip::inject {

Initialization::Initialization()
  : m_client_dll(nullptr), m_steam_api_dll(nullptr), m_steam_user(nullptr)
  , m_client_size(0), m_steam_api_size(0)
{ }

auto Initialization::init_window() -> bool {
  const auto m_game_window = GetModuleHandle(nullptr);

  if (!m_game_window)
    return false;
  
  const int m_game_pid = _getpid();

  if (!m_game_pid)
    return false;

  return true;
}

auto Initialization::init_main() -> std::optional<Interface> {
  if (!init_modules())
    return std::nullopt;

  Interface handles;
  
  handles.engine = reinterpret_cast<sdk::cl_enginefunc_t*>((DWORD)(m_client_dll) + ENGINE_OFFSET);

  auto pfn = sdk::get_init_game_connect_func(m_steam_user);  
  handles.init_game_connect = reinterpret_cast<sdk::InitiateGameConnectionFunc*>(pfn);

  pfn = sdk::get_term_game_connect_func(m_steam_user);
  handles.term_game_connect = reinterpret_cast<sdk::TerminateGameConnectionFunc*>(pfn);

  // fix for protected memory region
  RtlCopyMemory(&handles.engine, handles.engine, sizeof(handles.engine));

  return handles;
}

auto Initialization::init_modules() -> bool {
  const auto get_module_size = [](HMODULE address) {
    const auto addr = reinterpret_cast<uintptr_t>(address);
    const auto e_lfanew = PIMAGE_DOS_HEADER(addr)->e_lfanew;
    const auto nt_headers = PIMAGE_NT_HEADERS(addr + e_lfanew);
    return static_cast<uintptr_t>(nt_headers->OptionalHeader.SizeOfImage);
  };

  while (!m_client_dll) {
    m_client_dll = GetModuleHandle(CLIENT_DLL.data());
  }

  m_client_size = get_module_size(m_client_dll);
  
  while (!m_steam_api_dll) {
    m_steam_api_dll = GetModuleHandle(STEAM_API_DLL.data());
  }

  m_steam_api_size = get_module_size(m_steam_api_dll);

  const auto suf = reinterpret_cast<sdk::SteamUserFunctor>(GetProcAddress(m_steam_api_dll, STEAM_USER.data()));;

  if (!suf)
    return false;

  m_steam_user = suf();
  
  return true;
}

} // namespace sip::inject
