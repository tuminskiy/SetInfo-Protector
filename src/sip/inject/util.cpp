#include "sip/inject/util.hpp"

#include <chrono>
#include <ctime>
#include <string_view>

namespace sip::inject::util {

auto get_vtable(void* p_obj) -> void* {
  // represent pointer to object as array of pointers
  auto pointers = reinterpret_cast<void**>(p_obj);

  // the first pointer is a vtable pointer (in binaries compiled with msvc)
  return *pointers;
}

auto get_vtable_pfuncs(void* p_obj) -> void** {
  auto p_vtable = get_vtable(p_obj);

  // represent pointer to vtable as pointers to function pointers
  return reinterpret_cast<void**>(p_vtable); 
}

auto get_current_time() -> std::string {
  const auto timepoint = std::chrono::system_clock::now();
  const auto datetime = std::chrono::system_clock::to_time_t(timepoint);
  const auto tm = std::localtime(&datetime);

  constexpr std::string_view format = "%Y.%m.%d %H:%M:%S";

  std::string buffer;
  buffer.resize(19); // Format: 2023.05.19 14:49:02

  std::strftime(buffer.data(), buffer.size(), "%Y.%m.%d %H:%M:%S", tm);

  return buffer;
}

} // namespace sip::inject::util