#pragma once

#include "components/Taper.h"
#include "modules/curve-sequencer/AdvanceMode.h"
#include "modules/curve-sequencer/GenerateMode.h"

#include <functional>

namespace test {

namespace curve_sequencer {
  using dhe::curve_sequencer::AdvanceMode;
  using dhe::curve_sequencer::GenerateMode;
  using dhe::taper::VariableTaper;

  struct FakeControls {
    // Sequencer controls
    std::function<float()> input{[]() -> float { throw "FakeControls.input()"; }};
    std::function<bool()> isGated{[]() -> bool { throw "FakeControls.isGated()"; }};
    std::function<bool()> isLooping{[]() -> bool { throw "FakeControls.isLooping()"; }};
    std::function<bool()> isReset{[]() -> bool { throw "FakeControls.isReset()"; }};
    std::function<bool()> isRunning{[]() -> bool { throw "FakeControls.isRunning()"; }};
    std::function<void(float voltage)> setOutput{[](float f) { throw "FakeControls.output(voltage)"; }};
    std::function<float()> getOutput{[]() -> float { throw "FakeControls.output()"; }};

    auto output() const -> float { return getOutput(); }
    void output(float voltage) const { setOutput(voltage); };

    // Step controls
    std::function<GenerateMode(int step)> mode{[](int s) -> GenerateMode { throw "FakeControls.mode(step)"; }};
    std::function<AdvanceMode(int step)> condition{[](int s) -> AdvanceMode { throw "FakeControls.condition(step)"; }};

    std::function<float(int step)> curvature{[](int s) -> float { throw "FakeControls.curvature(step)"; }};
    std::function<float(int step)> duration{[](int s) -> float { throw "FakeControls.duration(step)"; }};
    std::function<float(int step)> level{[](int s) -> float { throw "FakeControls.level(step)"; }};
    std::function<VariableTaper const *(int step)> taper{
        [](int s) -> VariableTaper const * { throw "FakeControls.taper(step)"; }};

    std::function<void(int step, float progress)> showProgress{
        [](int s, float f) { throw "FakeControls.showProgress(step)"; }};
    std::function<void(int step)> showInactive{[](int s) { throw "FakeControls.showInactive(step)"; }};
  };

} // namespace curve_sequencer
} // namespace test
