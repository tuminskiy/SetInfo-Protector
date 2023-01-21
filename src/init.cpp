#include "sip/init.hpp"
#include "sip/const.hpp"
#include "sip/util.hpp"

#include <process.h>

namespace sip {

Initialization::Initialization()
  : m_game_window(nullptr), m_game_pid(0)
  , m_hardware_dll(nullptr), m_client_dll(nullptr), m_steam_api_dll(nullptr), m_steam_user(nullptr)
  , m_hardware_size(0), m_client_size(0), m_steam_api_size(0)
  , m_engine_sig(ENGINE_OFFSET, ENGINE_SIG)
{ }

auto Initialization::init_window() -> bool {
  m_game_window = GetModuleHandle(nullptr);

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

  handles.engine = reinterpret_cast<sdk::cl_enginefunc_t*>(
    Util::find_str_ref(m_client_dll, m_client_size, m_engine_sig)
  );

  // get pointer to InitiateGameConnection method address in virtual table of ISteamUser interface.

  /*
                         m_steam_user      --> pointer to ISteamUser class
                 (void**)m_steam_user      --> represent as array of pointers (VERY DIRTY type erasure)
                *(void**)m_steam_user      --> get pointer to vtable (in msvc the vtable pointer comes first in the class)
       (void**)(*(void**)m_steam_user)     --> represent as array of pointers (actually function pointers)
     ((void **)(*(void**)m_steam_user))[3] --> get third pointer to void (actually a function pointer)
    &((void **)(*(void**)m_steam_user))[3] --> get address of third pointer
  */
  auto pfn = &((void **)(*(void **)m_steam_user))[3];

  handles.init_game_connect = reinterpret_cast<sdk::InitiateGameConnectionFunc*>(pfn);

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

  m_hardware_dll = GetModuleHandle(HW_DLL.data());
  m_client_dll = GetModuleHandle(CLIENT_DLL.data());
  m_steam_api_dll = GetModuleHandle(STEAM_API_DLL.data());

  uint8_t hardware_type = 0;
  if (!m_hardware_dll) {
    hardware_type = 1;
    m_hardware_dll = GetModuleHandle(SW_DLL.data());
  }

  if (!m_hardware_dll) {
    hardware_type = 2;
    m_hardware_dll = GetModuleHandle(nullptr);
  }

  // failed to grab any of the module types
  if (!m_hardware_dll)
    return false;
  
  m_hardware_size = get_module_size(m_hardware_dll);

  // if we fail to grab the module size
  if (!m_hardware_size) {
    // set it to values we already knew beforehand
    switch (hardware_type) {
      case 0: m_hardware_size = 0x122A000; break; // hardware
      case 1: m_hardware_size = 0xB53000; break;  // software
      case 2: m_hardware_size = 0x2116000; break; // neither
    }
  }

  if (m_client_dll) {
    m_client_size = get_module_size(m_client_dll);
  } else {
    m_client_dll = m_hardware_dll;
    m_client_size = m_hardware_size;
  }

  sdk::SteamUserFunctor suf = nullptr;
  if (m_steam_api_dll) {
    m_steam_api_size = get_module_size(m_steam_api_dll);
    suf = reinterpret_cast<sdk::SteamUserFunctor>(GetProcAddress(m_steam_api_dll, STEAM_USER.data()));
  } else {
    m_steam_api_size = 0;
  }

  if (!suf)
    return false;

  m_steam_user = suf();
  
  if (!m_hardware_dll || !m_client_dll || !m_steam_api_dll || !m_steam_user)
    return false;

  return true;
}

} // namespace  sip
