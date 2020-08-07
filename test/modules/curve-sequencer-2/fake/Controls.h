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

      // Step controls
      std::function<TriggerMode(int step)> triggerMode{
          [](int s) -> TriggerMode { throw "Controls.triggerMode(step)"; }};
      std::function<bool(int)> interruptOnTrigger{[](int s) -> bool { throw "Controls.interruptOnTrigger(step)"; }};

      std::function<float(int step)> curvature{[](int s) -> float { throw "Controls.curvature(step)"; }};
      std::function<float(int step)> duration{[](int s) -> float { throw "Controls.duration(step)"; }};
      std::function<float(int step)> level{[](int s) -> float { throw "Controls.level(step)"; }};
      std::function<VariableTaper const *(int step)> taper{
          [](int s) -> VariableTaper const * { throw "Controls.taper(step)"; }};

      std::function<bool(int)> isEnabled{[](int s) -> bool { throw "Controls.isEnabled(step)"; }};
      std::function<void(int step, float progress)> showProgress{
          [](int s, float f) { throw "Controls.showProgress(step,progress)"; }};
      std::function<void(int step)> showInactive{[](int s) { throw "Controls.showInactive(step)"; }};
    };

  } // namespace fake
} // namespace curve_sequencer_2
} // namespace test