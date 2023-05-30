#pragma once

#include "sip/settings/address.hpp"

#include <string>

namespace sip::inject::util {

auto get_vtable_pfuncs(void* p_obj) -> void**;

auto get_current_time() -> std::string;

auto get_address_from_node(uint32_t unIPServer, std::uint16_t usPortServer) -> sip::settings::Address;

template <class BasicJsonType, class T>
auto get_value_or_default(const BasicJsonType& j, std::string_view field, T* out) -> void {
  if (!out || j.count(field) == 0 || j[field].is_null())
    return;
  
  j[field].get_to(*out);
}

} // namespace sip::inject::util