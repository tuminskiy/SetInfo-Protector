#include "sip/inject/interface.hpp"

#include <sstream>

namespace sip::inject {

auto Interface::console_print(std::string_view sv) -> void {
  engine->pfnConsolePrint(sv.data());
}

auto Interface::set_info(std::string_view key, std::string_view value) -> void {
  std::ostringstream oss;
  oss << "setinfo \"" << key << "\" \"" << value << "\"";

  const auto str = oss.str();

  engine->pfnClientCmd(str.data());
}

} // namespace sip::inject