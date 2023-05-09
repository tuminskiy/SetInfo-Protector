#pragma once

#include <string_view>

namespace sip::inject {

class SigHelper {
public:
  constexpr SigHelper() : m_offset(0), m_sig("") { }
  constexpr SigHelper(std::string_view sig) : m_offset(0), m_sig(sig) { }
  constexpr SigHelper(int offset, std::string_view sig) : m_offset(offset), m_sig(sig) { }

  auto offset() const -> int;
  auto sig() const -> const std::string_view;
  auto sig() -> std::string_view&;

private:
  int m_offset;
  std::string_view m_sig;
};

} // namespace sip::inject