#include "sdk/steamuser.hpp"
#include "sip/inject/util.hpp"

namespace sdk {

auto get_init_game_connect_func(ISteamUser* steam_user) -> void** {
  auto p_funcs = sip::inject::util::get_vtable_pfuncs(steam_user);
  return &p_funcs[3];
}

auto get_term_game_connect_func(ISteamUser* steam_user) -> void** {
  auto p_funcs = sip::inject::util::get_vtable_pfuncs(steam_user);
  return &p_funcs[4];
}

} // namespace sdk