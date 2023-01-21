#pragma once

#include "sdk/cdll.hpp"
#include "sdk/steamuser.hpp"

namespace sip {

struct Interface {
  sdk::cl_enginefunc_t* engine;
  sdk::InitiateGameConnectionFunc* init_game_connect;
};

} // namespace sip