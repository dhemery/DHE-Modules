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

  template <int N> class SustainStage {
    using Controls = CurveSequencerControls<N>;

  public:
    SustainStage(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
        inputs{inputs}, params{params}, lights{lights} {}

    void enter(int entryStep) {
      step = entryStep;
      setLight(true);
    }

    auto execute(dhe::Latch const &gateLatch) const -> SequencerState {
      if (isActive(runMode(), gateLatch)) {
        return {SequenceMode::Sustaining, step};
      }
      setLight(false);
      return {SequenceMode::Advancing, (step + 1) & stepMask};
    };

    void exit() { setLight(false); }

  private:
    auto runMode() const -> StageMode {
      return static_cast<StageMode>(params[Controls::SustainModeSwitches + step].getValue());
    }

    void setLight(const bool state) const {
      lights[Controls::SustainingLights + step].setBrightness(state ? 10.F : 0.F);
    }

    static auto constexpr stepMask = N - 1;
    std::vector<Input> &inputs;
    std::vector<Param> &params;
    std::vector<Light> &lights;
    int step{0};
  };
}; // namespace curve_sequencer
} // namespace dhe
