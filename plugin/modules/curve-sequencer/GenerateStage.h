#pragma once

#include "CurveSequencerControls.h"
#include "SequenceMode.h"
#include "SequencerState.h"
#include "StageMode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Param;

  template <int N> class GenerateStage {
    using Controls = CurveSequencerControls<N>;

  public:
    GenerateStage(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
        inputs{inputs}, params{params}, lights{lights} {}

    void enter(int entryStep) {
      step = entryStep;
      setLight(true);
    }

    auto execute(dhe::Latch const &gateLatch, float) const -> SequencerState {
      if (isActive(runMode(), gateLatch)) {
        return {SequenceMode::Generating, step};
      }
      setLight(false);
      return {SequenceMode::Sustaining, step};
    };

    void exit() { setLight(false); }

  private:
    auto runMode() const -> StageMode {
      return static_cast<StageMode>(params[Controls::GenerateModeSwitches + step].getValue());
    }

    void setLight(const bool state) const {
      lights[Controls::GeneratingLights + step].setBrightness(state ? 10.F : 0.F);
    }

    std::vector<Input> &inputs;
    std::vector<Param> &params;
    std::vector<Light> &lights;
    int step;
  };
}; // namespace curve_sequencer
} // namespace dhe
