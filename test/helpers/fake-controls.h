#pragma once

#include <string>
namespace test {
namespace fake {
  static inline auto forbidden(const std::string &name) -> std::string { return "Controls." + name + "()"; }

  template <typename V> static inline auto forbidden(const std::string &name, V v) -> std::string {
    return name + "(" + std::to_string(v) + ")";
  }

  template <typename V1, typename V2>
  static inline auto forbidden(const std::string &name, V1 v1, V2 v2) -> std::string {
    return name + "(" + std::to_string(v1) + "," + std::to_string(v2) + ")";
  }

  template <typename V1, typename V2, typename V3>
  static inline auto forbidden(const std::string &name, V1 v1, V2 v2, V3 v3) -> std::string {
    return name + "(" + std::to_string(v1) + "," + std::to_string(v2) + "," + std::to_string(v3) + ")";
  }

  template <typename V> auto funcReturning(V v) -> std::function<V()> {
    return [v]() -> V { return v; };
  }

  template <typename P1, typename V> auto funcReturning(V v) -> std::function<V(P1)> {
    return [v](P1 p1 __attribute__((unused))) -> V { return v; };
  }

  template <typename P1, typename P2, typename V> auto funcReturning(V v) -> std::function<V(P1,P2)> {
    return [v](P1 p1 __attribute__((unused)), P2 pt __attribute((unused))) -> V { return v; };
  }
} // namespace fake
} // namespace test
