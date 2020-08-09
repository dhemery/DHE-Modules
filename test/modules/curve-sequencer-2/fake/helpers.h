#pragma once

#include <string>
namespace dhe {
namespace test {
  namespace fake {
    static inline auto forbidden(const std::string &name) -> std::string { return "Controls." + name + "()"; }

    template <typename V> static inline auto forbidden(const std::string &name, V v) -> std::string {
      return "Controls." + name + "(" + std::to_string(v) + ")";
    }

    template <typename V1, typename V2>
    static inline auto forbidden(const std::string &name, V1 v1, V2 v2) -> std::string {
      return "Controls." + name + "(" + std::to_string(v1) + "," + std::to_string(v2) + ")";
    }

    template <typename V> auto funcReturning(V v) -> std::function<V()> {
      return [v]() -> V { return v; };
    }

    template <typename P1, typename V> auto funcReturning(V v) -> std::function<V(P1)> {
      return [v](P1 p1 __attribute__((unused))) -> V { return v; };
    }
  } // namespace fake
} // namespace test
} // namespace dhe
