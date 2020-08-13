#pragma once
#include "components/PhaseTimer.h"

namespace dhe {
namespace curve_sequencer_2 {
  template <typename Controls, typename Source> class Generator {
  public:
    Generator(Controls &controls, Source &startSource, Source &endSource) :
        controls{controls}, startSource{startSource}, endSource{endSource} {}

    void start(int startedStep) {
      step = startedStep;
      controls.showProgress(step, 0.F);
      startSource.snap(step);
      endSource.snap(step);
    }

    auto generate(float sampleTime) -> bool { return false; }

    void stop() { controls.showInactive(step); }

  private:
    Controls &controls;
    Source &startSource;
    Source &endSource;
    PhaseTimer timer{};
    int step{0};
  };
} // namespace curve_sequencer_2
} // namespace dhe
