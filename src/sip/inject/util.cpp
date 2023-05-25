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

  constexpr auto buffer_size = std::size("yyyy.mm.dd HH:MM:SS");
  char buffer[buffer_size];

  std::strftime(buffer, buffer_size, "%Y.%m.%d %H:%M:%S", tm);

  return buffer;
}

} // namespace sip::inject::util