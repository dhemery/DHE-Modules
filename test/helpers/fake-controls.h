#pragma once

#include <sstream>
#include <string>

namespace test {
namespace fake {
  static inline auto forbidden(const std::string &name) -> std::string {
    auto s = std::stringstream{};
    s << name << "()";
    return s.str();
  }

  template <typename V> static inline auto forbidden(const std::string &name, V v) -> std::string {
    auto s = std::stringstream{};
    s << name << "(" + std::to_string(v) << ")";
    return s.str();
  }

  template <typename V1, typename V2>
  static inline auto forbidden(const std::string &name, V1 v1, V2 v2) -> std::string {
    auto s = std::stringstream{};
    s << name << "(" + std::to_string(v1) << "," << std::to_string(v2) << ")";
    return s.str();
  }

  template <typename V1, typename V2, typename V3>
  static inline auto forbidden(const std::string &name, V1 v1, V2 v2, V3 v3) -> std::string {
    auto s = std::ostringstream{};
    s << name << "(" + std::to_string(v1) << "," << std::to_string(v2) << "," << std::to_string(v3) << ")";
    return s.str();
  }
} // namespace fake
} // namespace test
