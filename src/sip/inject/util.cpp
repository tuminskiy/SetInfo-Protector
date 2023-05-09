#include "sip/inject/util.hpp"

namespace sip::inject::util {

auto find_pattern_down(HMODULE hmodule, uintptr_t size, const SigHelper& sig) -> uintptr_t {
  if (!hmodule)
    return 0;

  const auto signature = sig.sig();
  const auto end = (reinterpret_cast<uintptr_t>(hmodule) + size) - signature.size();

  for (auto i = reinterpret_cast<uintptr_t>(hmodule); i < end; i++) {
    bool is_found = true;

    for (size_t index = 0; index < signature.size(); index++) {
      const char symbol = *reinterpret_cast<char*>(i + index);

      if (signature[index] != symbol) {
        is_found = false;
        break;
      }
    }

    if (is_found) {
      return i + sig.offset();
    }
  }

  return 0;
}

auto find_str_ref(HMODULE hmodule, uintptr_t size, const SigHelper& sig) -> uintptr_t {  
  char push_address[] = { 0x68, 0x00, 0x00, 0x00, 0x00, 0x00 };

  SigHelper txt(sig.sig());

  const auto str_def = find_pattern_down(hmodule, size, txt);

  if (!str_def)
    return 0;
  
  *reinterpret_cast<uintptr_t*>(&push_address[1]) = str_def;

  txt.sig() = push_address;
  const auto str_ref = find_pattern_down(hmodule, size, txt);
  
  return str_ref ? str_ref + sig.offset() : 0;
}

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


} // namespace sip::inject::util