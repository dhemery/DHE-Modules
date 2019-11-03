#pragma once

#include "CurveSequencerControls.h"
#include "Mode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Input;
  using rack::engine::Param;

  template <int N> class AdvancingMode : public Mode {
  public:
    AdvancingMode(std::vector<Input> &inputs, std::vector<Param> &params) : inputs{inputs}, params{params} {}
    auto isTerminal() const -> bool override { return false; }

    auto execute(dhe::Latch const & /*runLatch*/, dhe::Latch const & /*gateLatch*/, int start,
                 float /*sampleTime*/) const -> Successor override {
      auto const stop = selectionStart() + selectionLength();
      for (int step = start; step < stop; step++) {
        if (isEnabled(step)) {
          return {ModeId::Generating, step};
        }
      }
      return {ModeId::Idle, start};
    };

  private:
    using Controls = CurveSequencerControls<N>;

    auto selectionStart() const -> int { return static_cast<int>(valueOf(params[Controls::StartKnob])); }
    auto selectionLength() const -> int { return static_cast<int>(valueOf(params[Controls::StepsKnob])); }

    auto isEnabled(int step) const -> bool {
      return isPressed(params[Controls::EnabledButtons + step]) || isHigh(inputs[Controls::EnabledInputs + step]);
    }
    std::vector<Input> &inputs;
    std::vector<Param> &params;
  };
}; // namespace curve_sequencer
} // namespace dhe
