#pragma once

#include "Advancing.h"
#include "Controls.h"
#include "Generating.h"
#include "Idle.h"
#include "Mode.h"
#include "Sustaining.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType>
  class CurveSequencer {
    using Controls = Controls<N>;

  public:
    CurveSequencer(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                   std::vector<LightType> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    void execute(float sampleTime) {
      runLatch.clock(isRunning());
      gateLatch.clock(gate());

      if (runLatch.isLow()) {
        return;
      }

      do {
        auto const next = executeMode(sampleTime);
        if (next.mode == mode) {
          return;
        }
        updateState(next);
      } while (mode != Mode::Idle);
    }

  private:
    auto executeMode(float sampleTime) const -> Successor {
      switch (mode) {
      case Mode::Idle:
        return idleMode.execute(gateLatch);
      case Mode::Advancing:
        return advancingMode.execute(step);
      case Mode::Generating:
        return generatingMode.execute(gateLatch, sampleTime);
      case Mode::Sustaining:
        return sustainingMode.execute(gateLatch);
      }
    }

    auto gate() const -> bool { return isHigh(inputs[Controls::GateInput]) || isPressed(params[Controls::GateButton]); }

    auto isRunning() const -> bool {
      return isHigh(inputs[Controls::RunInput]) || isPressed(params[Controls::RunButton]);
    }

    void updateState(Successor next) {
      step = next.step;
      mode = next.mode;
      switch (next.mode) {
      case Mode::Generating:
        generatingMode.enter(step);
        return;
      case Mode::Sustaining:
        sustainingMode.enter(step);
        return;
      case Mode::Idle:
      case Mode::Advancing:
        return;
      }
    }

    int step{0};
    Latch runLatch{};
    Latch gateLatch{};
    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
    Mode mode{Mode::Idle};
    Advancing<N> advancingMode{inputs, params};
    Generating<N> generatingMode{inputs, params, lights};
    Idle<N> idleMode{params};
    Sustaining<N> sustainingMode{inputs, params, lights};
  };
} // namespace curve_sequencer
} // namespace dhe
