#pragma once

#include "sip/inject/util.hpp"

#include <string>
#include <string_view>

#include <nlohmann/json.hpp>


namespace sip::settings {


struct Cvar {
  std::string name;
  std::string value;
  std::string default;
};


} // namespace sip::settings



namespace nlohmann {

template <>
struct adl_serializer<sip::settings::Cvar> {

  static constexpr std::string_view CVAR_NAME_FIELD = "name";
  static constexpr std::string_view CVAR_VALUE_FIELD = "value";
  static constexpr std::string_view CVAR_DEFAULT_FIELD = "default";

  template <class BasicJsonType>
  static auto from_json(const BasicJsonType& j, sip::settings::Cvar& cvar) -> void {
    j.at(CVAR_NAME_FIELD).get_to(cvar.name);

    sip::inject::util::get_value_or_default(j, CVAR_VALUE_FIELD, &cvar.value);

    j.at(CVAR_DEFAULT_FIELD).get_to(cvar.default);
  }

  template <class BasicJsonType>
  static auto to_json(BasicJsonType& j, const sip::settings::Cvar& cvar) -> void {
    j[CVAR_NAME_FIELD] = cvar.name;
    j[CVAR_VALUE_FIELD] = cvar.value;
    j[CVAR_DEFAULT_FIELD] = cvar.default;
  }
};

} // namespace nlohmann