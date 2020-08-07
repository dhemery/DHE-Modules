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
      std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
      std::function<bool()> isGated{[]() -> bool { throw "FakeControls.isGated()"; }};
      std::function<bool()> isLooping{[]() -> bool { throw "FakeControls.isLooping()"; }};
      std::function<bool()> isReset{[]() -> bool { throw "FakeControls.isReset()"; }};
      std::function<bool()> isRunning{[]() -> bool { throw "FakeControls.isRunning()"; }};
      std::function<float()> getOutput{[]() -> float { throw "FakeControls.output()"; }};
      std::function<void(float voltage)> setOutput{[](float f) { throw "FakeControls.output(voltage)"; }};
      std::function<int()> selectionStart{[]() -> int { throw "FakeControls.selectionStart()"; }};
      std::function<int()> selectionLength{[]() -> int { throw "FakeControls.isSelectionLength()"; }};

      auto output() const -> float { return getOutput(); }
      void output(float voltage) const { setOutput(voltage); };

      // Step controls
      std::function<TriggerMode(int step)> triggerMode{
          [](int s) -> TriggerMode { throw "FakeControls.triggerMode(step)"; }};

      std::function<float(int step)> curvature{[](int s) -> float { throw "FakeControls.curvature(step)"; }};
      std::function<float(int step)> duration{[](int s) -> float { throw "FakeControls.duration(step)"; }};
      std::function<float(int step)> level{[](int s) -> float { throw "FakeControls.level(step)"; }};
      std::function<VariableTaper const *(int step)> taper{
          [](int s) -> VariableTaper const * { throw "FakeControls.taper(step)"; }};

      std::function<bool(int)> isEnabled{[](int s) -> bool { throw "FakeControls.isEnabled(step)"; }};
      std::function<void(int step, float progress)> showProgress{
          [](int s, float f) { throw "FakeControls.showProgress(step,progress)"; }};
      std::function<void(int step)> showInactive{[](int s) { throw "FakeControls.showInactive(step)"; }};
    };

  } // namespace fake
} // namespace curve_sequencer_2
} // namespace test