#pragma once

#include "components/Taper.h"
#include "modules/curve-sequencer-2/Source.h"
#include "modules/curve-sequencer-2/TriggerMode.h"

#include <functional>

namespace test {
namespace curve_sequencer_2 {
  namespace fake {
    using dhe::curve_sequencer_2::TriggerMode;
    using dhe::taper::VariableTaper;

    struct Controls {
      // Sequencer controls
      std::function<float()> input{[]() -> float { throw "Controls.input()"; }};
      std::function<bool()> isGated{[]() -> bool { throw "Controls.isGated()"; }};
      std::function<bool()> isLooping{[]() -> bool { throw "Controls.isLooping()"; }};
      std::function<bool()> isReset{[]() -> bool { throw "Controls.isReset()"; }};
      std::function<bool()> isRunning{[]() -> bool { throw "Controls.isRunning()"; }};
      std::function<float()> getOutput{[]() -> float { throw "Controls.output()"; }};
      std::function<void(float voltage)> setOutput{[](float f) { throw "Controls.output(voltage)"; }};
      std::function<int()> selectionStart{[]() -> int { throw "Controls.selectionStart()"; }};
      std::function<int()> selectionLength{[]() -> int { throw "Controls.isSelectionLength()"; }};

      auto output() const -> float { return getOutput(); }
      void output(float voltage) const { setOutput(voltage); };

      // Step advancement controls
      std::function<TriggerMode(int step)> triggerMode{
          [](int s) -> TriggerMode { throw "Controls.triggerMode(step)"; }};
      std::function<bool(int)> interruptOnTrigger{[](int s) -> bool { throw "Controls.interruptOnTrigger(step)"; }};
      std::function<bool(int)> advanceOnEndOfCurve{[](int s) -> bool { throw "Controls.advanceOnEndOfCurve(step)"; }};

      // Step generation controls
      std::function<float(int step)> curvature{[](int s) -> float { throw "Controls.curvature(step)"; }};
      std::function<float(int step)> duration{[](int s) -> float { throw "Controls.duration(step)"; }};
      std::function<float(int step)> startLevel{[](int s) -> float { throw "Controls.startLevel(step)"; }};
      std::function<float(int step)> endLevel{[](int s) -> float { throw "Controls.endLevel(step)"; }};
      std::function<VariableTaper const *(int step)> taper{
          [](int s) -> VariableTaper const * { throw "Controls.taper(step)"; }};

      // Step progress controls
      std::function<bool(int)> isEnabled{[](int s) -> bool { throw "Controls.isEnabled(step)"; }};
      std::function<void(int step, float progress)> showProgress{
          [](int s, float f) { throw "Controls.showProgress(step,progress)"; }};
      std::function<void(int step)> showInactive{[](int s) { throw "Controls.showInactive(step)"; }};
    };

    inline void allowGenerate(Controls &controls) {
      controls.advanceOnEndOfCurve = [](int s __attribute__((unused))) -> bool { return false; };
      controls.interruptOnTrigger = [](int s __attribute__((unused))) -> bool { return false; };
      controls.setOutput = [](float f __attribute__((unused))) {};
      controls.showProgress = [](int s, float f) {};

      controls.curvature = [](int s __attribute__((unused))) -> float { return 0.F; };
      controls.duration = [](int s __attribute__((unused))) -> float { return 0.F; };
      controls.input = []() -> float { return 0.F; };
      controls.startLevel = [](int s __attribute__((unused))) -> float { return 0.F; };
      controls.endLevel = [](int s __attribute__((unused))) -> float { return 0.F; };
      controls.taper
          = [](int s __attribute__((unused))) -> VariableTaper const * { return dhe::taper::variableTapers[0]; };
    }

    template <typename V> auto stepControlReturning(V v) -> std::function<V(int)> {
      return [v](int s __attribute__((unused))) -> V { return v; };
    }
  } // namespace fake
} // namespace curve_sequencer_2
} // namespace test
