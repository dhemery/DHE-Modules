#pragma once

#include "components/Taper.h"
#include "helpers/fake-controls.h"
#include "modules/curve-sequencer-2/Source.h"
#include "modules/curve-sequencer-2/TriggerMode.h"

#include <functional>
#include <string>

namespace test {
namespace fake {
  using dhe::curve_sequencer_2::TriggerMode;
  using TaperT = dhe::taper::VariableTaper const *;

  struct Controls {
    // Sequencer controls
    auto output() const -> float { return getOutput(); }
    void output(float voltage) const { setOutput(voltage); };
    std::function<float()> input{[]() -> float { throw forbidden("input"); }};
    std::function<bool()> isGated{[]() -> bool { throw forbidden("isGated"); }};
    std::function<bool()> isLooping{[]() -> bool { throw forbidden("isLooping"); }};
    std::function<bool()> isReset{[]() -> bool { throw forbidden("isReset"); }};
    std::function<bool()> isRunning{[]() -> bool { throw forbidden("isRunning"); }};
    std::function<float()> getOutput{[]() -> float { throw forbidden("output"); }};
    std::function<void(float)> setOutput{[](float f) { throw forbidden("output", f); }};
    std::function<int()> selectionStart{[]() -> int { throw forbidden("selectionStart"); }};
    std::function<int()> selectionLength{[]() -> int { throw forbidden("selectionLength"); }};

    // Step advancement controls
    std::function<TriggerMode(int)> triggerMode{[](int s) -> TriggerMode { throw forbidden("triggerMode", s); }};

    // Step curve controls
    std::function<float(int)> curvature{[](int s) -> float { throw forbidden("curvature", s); }};
    std::function<float(int)> duration{[](int s) -> float { throw forbidden("duration", s); }};
    std::function<float(int)> endLevel{[](int s) -> float { throw forbidden("endLevel", s); }};
    std::function<float(int)> startLevel{[](int s) -> float { throw forbidden("startLevel", s); }};
    std::function<TaperT(int)> taper{[](int s) -> TaperT { throw forbidden("taper", s); }};

    // Step progress controls
    std::function<bool(int)> isEnabled{[](int s) -> bool { throw forbidden("isEnabled", s); }};
    std::function<void(int)> showInactive{[](int s) { throw forbidden("showInactive", s); }};
    std::function<void(int, float)> showProgress{[](int s, float f) { throw forbidden("showProgress", s, f); }};
  };

  template <typename V> auto controlReturning(V v) -> std::function<V()> {
    return [v]() -> V { return v; };
  }

  template <typename V> auto stepControlReturning(V v) -> std::function<V(int)> {
    return [v](int s __attribute__((unused))) -> V { return v; };
  }

  inline void allowGenerate(fake::Controls &controls) {
    controls.input = controlReturning(0.F);
    controls.setOutput = [](float f) {};
    controls.showProgress = [](int s, float f) {};

    controls.curvature = stepControlReturning(0.F);
    controls.duration = stepControlReturning(0.F);
    controls.startLevel = stepControlReturning(0.F);
    controls.endLevel = stepControlReturning(0.F);
    controls.taper = stepControlReturning(&dhe::taper::variableJTaper);
  }
} // namespace fake
} // namespace test
