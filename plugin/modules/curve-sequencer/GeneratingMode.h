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
  static auto constexpr generateInterruptModeCount = static_cast<int>(InterruptMode::Duration) + 1;
  static auto constexpr defaultGenerateinterruptMode = static_cast<int>(InterruptMode::Duration);
  static auto const generateInterruptModeDescriptions = std::array<std::string, generateInterruptModeCount>{
      "Advance if gate rises",       "Advance if gate falls",  "Advance if gate changes",
      "Advance if gate is high",     "Advance if gate is low", "Skip curve",
      "Advance when curve completes"};

  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Param;

  template <int N> class GeneratingMode {
    using Controls = CurveSequencerControls<N>;

  public:
    GeneratingMode(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
        inputs{inputs}, params{params}, lights{lights} {}

    void enter(int entryStep) {
      step = entryStep;
      setLight(true);
    }

    auto execute(dhe::Latch const &gateLatch, float) const -> Successor {
      if (interrupted(interruptMode(), gateLatch)) {
        setLight(false);
        return {ModeId::Advancing, step};
      }
      return {ModeId::Generating, step};
    };

    void exit() { setLight(false); }

  private:
    auto interruptMode() const -> InterruptMode {
      return static_cast<InterruptMode>(params[Controls::GenerateModeSwitches + step].getValue());
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
