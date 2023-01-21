#pragma once

#include <cstdint>
#include <Windows.h>

namespace sdk {

class ISteamUser;

using SteamUserFunctor = ISteamUser*(*)();

using InitiateGameConnectionFunc = int(__fastcall*)(ISteamUser *self, int cbMaxAuthBlob, void *pData, int cbMaxData,
  long long steamID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure);

} // namespace sdk