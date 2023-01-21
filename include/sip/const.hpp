#pragma once

#include <string_view>

namespace sip {

constexpr std::string_view HW_DLL = "hw.dll";
constexpr std::string_view CLIENT_DLL = "client.dll";
constexpr std::string_view STEAM_API_DLL = "steam_api.dll";
constexpr std::string_view SW_DLL = "sw.dll";
constexpr std::string_view STEAM_USER = "SteamUser";
constexpr std::string_view ENGINE_SIG = "sprites/%s.spr";
constexpr int ENGINE_OFFSET = 0x12;

} // namespace sip