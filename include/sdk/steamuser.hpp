#pragma once

#include <cstdint>
#include <Windows.h>

namespace sdk {

class ISteamUser;

using SteamUserFunctor = ISteamUser*(*)();

using InitiateGameConnectionFunc = int(__fastcall*)(void* pAuthBlob, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure);

using TerminateGameConnectionFunc = void(__fastcall*)(void* pAuthBlob, int cbMaxAuthBlob, uint32_t unIPServer, uint16_t usPortServer);

auto get_init_game_connect_func(ISteamUser* steamUser) -> void**;

auto get_term_game_connect_func(ISteamUser* steamUser) -> void**;

} // namespace sdk