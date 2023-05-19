#pragma once

#include "sdk/cdll.hpp"
#include "sdk/steamuser.hpp"

#include <string_view>

namespace sip::inject {

struct Interface {
  sdk::cl_enginefunc_t* engine;
  sdk::InitiateGameConnectionFunc* init_game_connect;
  sdk::TerminateGameConnectionFunc* term_game_connect;

  template <class... Args>
  auto console_print(std::string_view format, Args&&... args) -> void;

  auto set_info(std::string_view key, std::string_view value) -> void;

  auto set_filterstuffcmd(bool state) -> void;
};


template <class... Args>
auto Interface::console_print(std::string_view format, Args&&... args) -> void {
  engine->Con_Printf(format.data(), args...);
}

} // namespace sip::inject