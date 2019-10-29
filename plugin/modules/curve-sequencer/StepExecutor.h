#pragma once

#include "CurveSequencerControls.h"
#include "Stages.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename I, typename O, typename P, typename L, typename GenerateStage, typename SustainStage>
  class StepExecutor {
  public:
    StepExecutor(std::vector<I> &inputs, std::vector<O> &outputs, std::vector<P> &params, std::vector<L> &lights,
                 GenerateStage &generateStage, SustainStage &sustainStage) :
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
      return generateStage.execute(stepIndex, gateLatch, sampleTime) || sustainStage.execute(stepIndex, gateLatch);
    }

    auto isEnabled(int stepIndex) const -> bool {
      return isHigh(inputs[CurveSequencerControls<N>::EnabledInputs + stepIndex])
             || isPressed(params[CurveSequencerControls<N>::EnabledButtons + stepIndex]);
    }

  private:
    std::vector<I> &inputs;
    std::vector<O> &outputs;
    std::vector<P> &params;
    std::vector<L> &lights;
    GenerateStage &generateStage;
    SustainStage &sustainStage;
  };
} // namespace curve_sequencer
} // namespace dhe
