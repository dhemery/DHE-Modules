#pragma once

#include "CurveSequencerControls.h"
#include "SequenceMode.h"
#include "SequencerState.h"
#include "StepMode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  static auto constexpr sustainingInterruptModeCount = static_cast<int>(StepMode::Skip) + 1;
  static auto constexpr defaultSustainingInterruptMode = static_cast<int>(StepMode::Skip);
  static auto const sustainingInterruptModeDescriptions = std::array<std::string, sustainingInterruptModeCount>{
      "Sustain while gate is high", "Sustain while gate is low", "Sustain while gate is calm", "Skip sustain"};

  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Param;

  template <int N> class Sustaining {
    using Controls = CurveSequencerControls<N>;

  public:
    Sustaining(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
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
    auto runMode() const -> StepMode {
      return static_cast<StepMode>(params[Controls::SustainModeSwitches + step].getValue());
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
