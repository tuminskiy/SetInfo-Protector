#pragma once

#include <string>

namespace sip::inject::util {

auto get_vtable_pfuncs(void* p_obj) -> void**;

auto get_current_time() -> std::string;

} // namespace sip::inject::util