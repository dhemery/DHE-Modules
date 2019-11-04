#pragma once

#include "AdvancingMode.h"
#include "CurveSequencerControls.h"
#include "GeneratingMode.h"
#include "IdleMode.h"
#include "Mode.h"
#include "PauseMode.h"
#include "SustainingMode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType>
  class CurveSequencer {
    using Controls = CurveSequencerControls<N>;

  public:
    CurveSequencer(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                   std::vector<LightType> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    void execute(float sampleTime) {
      runLatch.clock(isRunning());
      gateLatch.clock(gate());

      do {
        auto const next = executeMode(sampleTime);
        if (next.mode == mode) {
          return;
        }
        updateState(next);
      } while (mode != Mode::Paused && mode != Mode::Idle);
    }

  private:
    auto executeMode(float sampleTime) const -> Successor {
      switch (mode) {
      case Mode::Paused:
        return pauseMode.execute(runLatch);
      case Mode::Idle:
        return idleMode.execute(runLatch, gateLatch);
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
      case Mode::Paused:
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
    Mode mode{Mode::Paused};
    AdvancingMode<N> advancingMode{inputs, params};
    GeneratingMode<N> generatingMode{inputs, params, lights};
    IdleMode<N> idleMode{params};
    PauseMode pauseMode{};
    SustainingMode<N> sustainingMode{inputs, params, lights};
  };
} // namespace curve_sequencer
} // namespace dhe
