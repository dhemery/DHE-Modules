#pragma once

#include "CurveSequencerControls.h"
#include "InterruptModes.h"
#include "Mode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  static auto constexpr sustainInterruptModeCount = static_cast<int>(InterruptMode::Skip) + 1;
  static auto constexpr defaultSustainInterruptMode = static_cast<int>(InterruptMode::Skip);
  static auto const sustainInterruptModeDescriptions = std::array<std::string, sustainInterruptModeCount>{
      "Sustain until gate rises",   "Sustain until gate falls",  "Sustain until gate changes",
      "Sustain until gate is high", "Sustain until gate is low", "Skip sustain"};

  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Param;

  template <int N> class SustainingMode {
    using Controls = CurveSequencerControls<N>;

  public:
    SustainingMode(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
        inputs{inputs}, params{params}, lights{lights} {}

    void enter(int entryStep) {
      step = entryStep;
      setLight(true);
    }

    auto execute(dhe::Latch const &gateLatch) const -> Successor {
      if (interrupted(gateLatch)) {
        setLight(false);
        return {ModeId::Advancing, step};
      }
      return {ModeId::Sustaining, step};
    };

    void exit() { setLight(false); }

  private:
    auto interrupted(Latch const &gate) const -> bool {
      auto const interruptMode = static_cast<InterruptMode>(params[Controls::SustainModeSwitches + step].getValue());
      return dhe::curve_sequencer::interrupted(interruptMode, gate);
    }

    void setLight(const bool state) const {
      lights[Controls::SustainingLights + step].setBrightness(state ? 10.F : 0.F);
    }

    std::vector<Input> &inputs;
    std::vector<Param> &params;
    std::vector<Light> &lights;
    int step{0};
  };
}; // namespace curve_sequencer
} // namespace dhe
