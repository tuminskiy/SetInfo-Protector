#pragma once

#include "sip/settings/address.hpp"

#include <string>

namespace sip::inject::util {

auto get_vtable_pfuncs(void* p_obj) -> void**;

auto get_current_time() -> std::string;

auto get_address_from_node(uint32_t unIPServer, std::uint16_t usPortServer) -> sip::settings::Address;

} // namespace sip::inject::util