#pragma once

#include "modules/curve-sequencer-2/source.h"
#include "modules/curve-sequencer-2/triggers.h"

#include <functional>
#include <string>

namespace test {
namespace old_curve_sequencer {
static inline auto forbidden(const std::string &name) -> std::string {
  return "Controls." + name + "()";
}

template <typename V>
static inline auto forbidden(const std::string &name, V v) -> std::string {
  return "Controls." + name + "(" + std::to_string(v) + ")";
}

template <typename V1, typename V2>
static inline auto forbidden(const std::string &name, V1 v1, V2 v2)
    -> std::string {
  return "Controls." + name + "(" + std::to_string(v1) + "," +
         std::to_string(v2) + ")";
}

namespace fake {
struct SequenceControls {
  std::function<bool()> is_gated{[]() -> bool { throw forbidden("is_gated"); }};
  std::function<bool()> is_looping{
      []() -> bool { throw forbidden("is_looping"); }};
  std::function<bool()> is_reset{[]() -> bool { throw forbidden("is_reset"); }};
  std::function<bool()> is_running{
      []() -> bool { throw forbidden("is_running"); }};
  std::function<int()> selection_start{
      []() -> int { throw forbidden("selection_start"); }};
  std::function<int()> selection_length{
      []() -> int { throw forbidden("selection_length"); }};
  std::function<float()> input{[]() -> int { throw forbidden("input"); }};
  std::function<void(float)> output{
      [](float f) { throw forbidden("output", f); }};
  std::function<bool(int)> is_enabled{
      [](int s) -> bool { throw forbidden("is_enabled", s); }};
};
} // namespace fake

template <typename V> auto controlReturning(V v) -> std::function<V()> {
  return [v]() -> V { return v; };
}

template <typename V> auto stepControlReturning(V v) -> std::function<V(int)> {
  return [v](int s __attribute__((unused))) -> V { return v; };
}
} // namespace curve_sequencer
} // namespace test
