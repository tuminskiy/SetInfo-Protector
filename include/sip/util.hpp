#pragma once

#include <Windows.h>
#include "sip/sighelper.hpp"

namespace sip {

class Util {
public:
  static auto find_str_ref(HMODULE hmodule, uintptr_t size, const SigHelper& sig) -> uintptr_t;
  
  static auto get_ip_from_node(uint32_t unIPServer, uint16_t usPortServer) -> std::string;

  static auto get_vtable_pfuncs(void* p_obj) -> void**;

private:
  static auto find_pattern_down(HMODULE hmodule, uintptr_t size, const SigHelper& sig) -> uintptr_t;
  static auto get_vtable(void* p_obj) -> void*;
};

} // namespace sip