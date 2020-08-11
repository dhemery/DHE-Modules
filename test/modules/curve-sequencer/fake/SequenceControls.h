#pragma once

#include "components/Taper.h"
#include "modules/curve-sequencer-2/Source.h"
#include "modules/curve-sequencer-2/Triggers.h"

#include <functional>
#include <string>

namespace test {
namespace curve_sequencer {
  static inline auto forbidden(const std::string &name) -> std::string { return "Controls." + name + "()"; }

  template <typename V> static inline auto forbidden(const std::string &name, V v) -> std::string {
    return "Controls." + name + "(" + std::to_string(v) + ")";
  }

  template <typename V1, typename V2>
  static inline auto forbidden(const std::string &name, V1 v1, V2 v2) -> std::string {
    return "Controls." + name + "(" + std::to_string(v1) + "," + std::to_string(v2) + ")";
  }

  namespace fake {
    struct SequenceControls {
      std::function<bool()> isGated{[]() -> bool { throw forbidden("isGated"); }};
      std::function<bool()> isLooping{[]() -> bool { throw forbidden("isLooping"); }};
      std::function<bool()> isReset{[]() -> bool { throw forbidden("isReset"); }};
      std::function<bool()> isRunning{[]() -> bool { throw forbidden("isRunning"); }};
      std::function<int()> selectionStart{[]() -> int { throw forbidden("selectionStart"); }};
      std::function<int()> selectionLength{[]() -> int { throw forbidden("selectionLength"); }};
      std::function<float()> input{[]() -> int { throw forbidden("input"); }};
      std::function<void(float)> output{[](float f) { throw forbidden("output", f); }};
      std::function<bool(int)> isEnabled{[](int s) -> bool { throw forbidden("isEnabled", s); }};
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
