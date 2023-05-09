#pragma once

#include "sdk/cdll.hpp"
#include "sdk/steamuser.hpp"

#include <string_view>

namespace sip::inject {

struct Interface {
  sdk::cl_enginefunc_t* engine;
  sdk::InitiateGameConnectionFunc* init_game_connect;
  sdk::TerminateGameConnectionFunc* term_game_connect;

  auto console_print(std::string_view str) -> void;
  auto set_info(std::string_view key, std::string_view value) -> void;
};

} // namespace sip::inject