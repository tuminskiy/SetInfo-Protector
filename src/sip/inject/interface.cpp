#include "sip/inject/interface.hpp"

namespace sip::inject {

auto Interface::set_info(std::string_view key, std::string_view value) -> void {
  engine->PlayerInfo_SetValueForKey(key.data(), value.data());
}

auto Interface::set_filterstuffcmd(bool state) -> void {
  engine->Cvar_Set("cl_filterstuffcmd", state ? "1" : "0");
}

} // namespace sip::inject