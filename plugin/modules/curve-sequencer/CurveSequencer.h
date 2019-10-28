#pragma once

#include "CurveSequencerControls.h"
#include "StepExecutor.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <memory>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType>
  class CurveSequencer {
    using StepExecutor = StepExecutor<N, InputType, OutputType, ParamType, LightType>;

  public:
    CurveSequencer(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                   std::vector<LightType> &lights, StepExecutor *stepExecutor) :
        inputs{inputs},
        outputs{outputs},
        params{params},
        lights{lights},
        stepExecutor{stepExecutor} {}

    CurveSequencer(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                   std::vector<LightType> &lights) :
        CurveSequencer(inputs, outputs, params, lights, new StepExecutor(inputs, outputs, params, lights)) {}

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
        if (stepExecutor->execute(activeStep, gateLatch, sampleTime)) {
          return;
        }
      }
      isActive = false;
    }

  private:
    auto gate() const -> bool {
      return isHigh(inputs[CurveSequencerControls<N>::GateInput])
             || isPressed(params[CurveSequencerControls<N>::GateButton]);
    }

    auto isRunning() const -> bool {
      return isHigh(inputs[CurveSequencerControls<N>::RunInput])
             || isPressed(params[CurveSequencerControls<N>::RunButton]);
    }

    auto selectionLength() const -> int { return valueOf(params[CurveSequencerControls<N>::StepsKnob]); }

    auto selectionStart() const -> int { return valueOf(params[CurveSequencerControls<N>::StartKnob]) - 1; }

    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
    bool isActive{};
    int activeStep{};
    Latch runLatch{};
    Latch gateLatch{};
    int const stepIndexMask = N - 1;
    std::unique_ptr<StepExecutor> stepExecutor;
  };
} // namespace curve_sequencer
} // namespace dhe
