#include "sip/inject/sighelper.hpp"

namespace sip::inject {

auto SigHelper::offset() const -> int { return m_offset; }

auto SigHelper::sig() const -> const std::string_view { return m_sig; }

auto SigHelper::sig() -> std::string_view& { return m_sig; }

} // namespace sip::inject