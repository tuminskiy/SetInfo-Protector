#include "sip/inject/util.hpp"

#include <chrono>
#include <ctime>
#include <string_view>

namespace {


auto get_vtable(void* p_obj) -> void* {
  // represent pointer to object as array of pointers
  auto pointers = reinterpret_cast<void**>(p_obj);

  // the first pointer is a vtable pointer (in binaries compiled with msvc)
  return *pointers;
}


} // anonymus namespace

namespace sip::inject::util {


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


auto get_address_from_node(uint32_t unIPServer, std::uint16_t usPortServer) -> sip::settings::Address {
  constexpr uint32_t node_a_mask = 0b1111'1111'0000'0000'0000'0000'0000'0000;
  constexpr uint32_t node_b_mask = 0b0000'0000'1111'1111'0000'0000'0000'0000;
  constexpr uint32_t node_c_mask = 0b0000'0000'0000'0000'1111'1111'0000'0000;
  constexpr uint32_t node_d_mask = 0b0000'0000'0000'0000'0000'0000'1111'1111;

  const uint8_t node_a = (unIPServer & node_a_mask) >> 24;
  const uint8_t node_b = (unIPServer & node_b_mask) >> 16;
  const uint8_t node_c = (unIPServer & node_c_mask) >> 8;
  const uint8_t node_d = (unIPServer & node_d_mask);

  sip::settings::Address address;
  address.ip = std::to_string(node_a) + "." + std::to_string(node_b) + "." + std::to_string(node_c) + "." + std::to_string(node_d);
  address.port = usPortServer;

  return address;
}


} // namespace sip::inject::util