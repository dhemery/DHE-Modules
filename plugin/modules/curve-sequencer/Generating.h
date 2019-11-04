#pragma once

#include "CurveSequencerControls.h"
#include "Mode.h"
#include "RunModes.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  static auto constexpr generatingInterruptModeCount = static_cast<int>(RunMode::Generate) + 1;
  static auto constexpr defaultGeneratingInterruptMode = static_cast<int>(RunMode::Generate);
  static auto const generatingInterruptModeDescriptions = std::array<std::string, generatingInterruptModeCount>{
      "Generate while gate is high", "Generate while gate is low", "Generate while gate is calm", "Skip generating",
      "Generate until done"};

  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Param;

  template <int N> class Generating {
    using Controls = CurveSequencerControls<N>;

  public:
    Generating(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
        inputs{inputs}, params{params}, lights{lights} {}

    void enter(int entryStep) {
      step = entryStep;
      setLight(true);
    }

    auto execute(dhe::Latch const &gateLatch, float) const -> Successor {
      if (isRunning(runMode(), gateLatch)) {
        return {Mode::Generating, step};
      }
      setLight(false);
      return {Mode::Sustaining, step};
    };

    void exit() { setLight(false); }

  private:
    auto runMode() const -> RunMode {
      return static_cast<RunMode>(params[Controls::GenerateModeSwitches + step].getValue());
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
