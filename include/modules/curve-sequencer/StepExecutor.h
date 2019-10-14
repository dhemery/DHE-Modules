#pragma once

#include "modules/components/Latch.h"

#include <memory>

namespace dhe {
namespace curve_sequencer {
  template <typename C> class GenerateStage {
  public:
    auto execute(int, Latch const &, float) -> bool { return false; }
  };
  template <typename C> class SustainStage {
  public:
    auto execute(int, Latch const &) -> bool { return false; }
  };

  template <typename C, typename G = GenerateStage<C>, typename S = SustainStage<C>> class StepExecutor {
  public:
    explicit StepExecutor(C &controls, G *generateStage, S *sustainStage) :
        controls{controls}, generateStage{generateStage}, sustainStage{sustainStage} {}
    explicit StepExecutor(C &controls) : StepExecutor{controls, new G, new S} {}

    auto execute(int stepIndex, Latch const &gateLatch, float sampleTime) -> bool {
      if (!controls.isEnabled(stepIndex)) {
        return false;
      }
      return generateStage->execute(stepIndex, gateLatch, sampleTime) || sustainStage->execute(stepIndex, gateLatch);
    }

  private:
    C &controls;
    std::unique_ptr<G> generateStage;
    std::unique_ptr<S> sustainStage;
  };
} // namespace curve_sequencer
} // namespace dhe
