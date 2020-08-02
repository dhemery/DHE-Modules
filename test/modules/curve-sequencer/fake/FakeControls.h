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
    std::function<float()> input;
    std::function<bool()> isGated;
    std::function<bool()> isLooping;
    std::function<bool()> isReset;
    std::function<bool()> isRunning;
    std::function<void(float voltage)> setOutput;
    std::function<float()> getOutput;

    auto output() const -> float { return getOutput(); }
    void output(float voltage) const { setOutput(voltage); };

    std::function<GenerateMode(int step)> mode;
    std::function<AdvanceMode(int step)> condition;

    std::function<float(int step)> curvature;
    std::function<float(int step)> duration;
    std::function<float(int step)> level;
    std::function<VariableTaper const *(int step)> taper;

    std::function<void(int step, float progress)> showProgress;
    std::function<void(int step)> showInactive;
  };

} // namespace curve_sequencer
} // namespace test
