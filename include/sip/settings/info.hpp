#pragma once

#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

namespace sip::settings {

struct Info {
  std::string key;
  std::string value;
};

} // namespace sip::settings

namespace {

constexpr std::string_view INFO_KEY_FIELD = "key";
constexpr std::string_view INFO_VALUE_FIELD = "value";

} // anonymous namespace

namespace nlohmann {

template <>
struct adl_serializer<sip::settings::Info> {

  template <class BasicJsonType>
  static auto from_json(const BasicJsonType& j, sip::settings::Info& info) -> void {
    j.at(INFO_KEY_FIELD).get_to(info.key);
    j.at(INFO_VALUE_FIELD).get_to(info.value);
  }

  template <class BasicJsonType>
  static auto to_json(BasicJsonType& j, const sip::settings::Info& info) -> void {
    j[INFO_KEY_FIELD] = info.key;
    j[INFO_VALUE_FIELD] = info.value;
  }

};

} // namespace nlohmann