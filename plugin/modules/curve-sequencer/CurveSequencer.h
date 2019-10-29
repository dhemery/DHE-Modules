#pragma once

#include "CurveSequencerControls.h"
#include "StepExecutor.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <memory>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType,
            typename StepExecutorType>
  class CurveSequencer {
    using Controls = CurveSequencerControls<N>;

  public:
    CurveSequencer(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                   std::vector<LightType> &lights, StepExecutorType &stepExecutor) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights}, stepExecutor{stepExecutor} {}

    void execute(float sampleTime) {
      runLatch.clock(isRunning());
      gateLatch.clock(gate());

      auto const start = selectionStart();

      if (!isActive) {
        if (gateLatch.isRise()) {
          isActive = true;
          activeStep = start;
        } else {
          return;
        }
      }

      auto const length = selectionLength();
      auto const end = start + length - 1;

      for (int i = activeStep; i <= end; i++) {
        activeStep = i & stepIndexMask;
        if (stepExecutor.execute(activeStep, gateLatch, sampleTime)) {
          return;
        }
      }
      isActive = false;
    }

  private:
    auto gate() const -> bool { return isHigh(inputs[Controls::GateInput]) || isPressed(params[Controls::GateButton]); }

    auto isRunning() const -> bool {
      return isHigh(inputs[Controls::RunInput]) || isPressed(params[Controls::RunButton]);
    }

    auto selectionLength() const -> int { return valueOf(params[Controls::StepsKnob]); }

    auto selectionStart() const -> int { return valueOf(params[Controls::StartKnob]) - 1; }

    bool isActive{};
    int activeStep{};
    Latch runLatch{};
    Latch gateLatch{};
    int const stepIndexMask = N - 1;
    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
    StepExecutorType &stepExecutor;
  };
} // namespace curve_sequencer
} // namespace dhe
