#pragma once

#include <string_view>

namespace sip::inject {

constexpr std::string_view CLIENT_DLL = "client.dll";
constexpr std::string_view STEAM_API_DLL = "steam_api.dll";
constexpr std::string_view STEAM_USER = "SteamUser";
constexpr uint32_t ENGINE_OFFSET = 0x3FCB9;

} // namespace sip::inject