#include "sip/inject/interface.hpp"

namespace sip::inject {

auto Interface::set_info(std::string_view key, std::string_view value) -> void {
  engine->PlayerInfo_SetValueForKey(key.data(), value.data());
}

auto Interface::set_cvar(std::string_view name, std::string_view value) -> void {
  engine->Cvar_Set(const_cast<char*>(name.data()), const_cast<char*>(value.data()));
}

} // namespace sip::inject