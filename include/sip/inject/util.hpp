#pragma once

#include "sip/inject/sighelper.hpp"

#include <Windows.h>

namespace sip::inject::util {

auto find_str_ref(HMODULE hmodule, uintptr_t size, const SigHelper& sig) -> uintptr_t;

auto get_vtable_pfuncs(void* p_obj) -> void**;

} // namespace sip::inject::util