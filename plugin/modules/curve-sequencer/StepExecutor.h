#pragma once

#include "CurveSequencerControls.h"
#include "Stages.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <memory>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType,
            typename GenerateStageType, typename SustainStageType>
  class StepExecutor {
    using GenerateStage = GenerateStage<N, InputType, OutputType, ParamType, LightType>;
    using SustainStage = SustainStage<N, InputType, OutputType, ParamType, LightType>;

  public:
    explicit StepExecutor(std::vector<InputType> &inputs, std::vector<OutputType> &outputs,
                          std::vector<ParamType> &params, std::vector<LightType> &lights) :
        StepExecutor{inputs,
                     outputs,
                     params,
                     lights,
                     new GenerateStage(inputs, outputs, params, lights),
                     new SustainStage(inputs, outputs, params, lights)} {}

    StepExecutor(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                 std::vector<LightType> &lights, GenerateStageType *generateStage, SustainStageType *sustainStage) :
        inputs{inputs},
        outputs{outputs},
        params{params},
        lights{lights},
        generateStage{generateStage},
        sustainStage{sustainStage} {}

    auto execute(int stepIndex, Latch const &gateLatch, float sampleTime) -> bool {
      if (!isEnabled(stepIndex)) {
        return false;
      }
      return generateStage->execute(stepIndex, gateLatch, sampleTime) || sustainStage->execute(stepIndex, gateLatch);
    }

    auto isEnabled(int stepIndex) const -> bool {
      return isHigh(inputs[CurveSequencerControls<N>::EnabledInputs + stepIndex])
             || isPressed(params[CurveSequencerControls<N>::EnabledButtons + stepIndex]);
    }

  private:
    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
    std::unique_ptr<GenerateStageType> generateStage;
    std::unique_ptr<SustainStageType> sustainStage;
  };
} // namespace curve_sequencer
} // namespace dhe
