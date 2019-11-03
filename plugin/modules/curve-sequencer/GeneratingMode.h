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
  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Param;

  template <int N> class GeneratingMode : public Mode {
    using Controls = CurveSequencerControls<N>;

  public:
    GeneratingMode(std::vector<Input> &inputs, std::vector<Param> &params, std::vector<Light> &lights) :
        inputs{inputs}, params{params}, lights{lights} {}

    auto isTerminal() const -> bool override { return false; }

    auto execute(dhe::Latch const & /*runLatch*/, dhe::Latch const &gateLatch, int step, float /*sampleTime*/) const
        -> Successor override {
      if (isActive(interruptMode(step), gateLatch)) {
        return {ModeId::Generating, step};
      }
      return {ModeId::Advancing, step};
    };

  private:
    auto interruptMode(int step) const -> InterruptMode {
      return static_cast<InterruptMode>(params[Controls::SustainModeSwitches + step].getValue());
    }

    std::vector<Input> &inputs;
    std::vector<Param> &params;
    std::vector<Light> &lights;
  };
}; // namespace curve_sequencer
} // namespace dhe
