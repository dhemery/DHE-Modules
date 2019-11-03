#pragma once

#include "CurveSequencerControls.h"
#include "Mode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Param.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Param;

  template <int N> class IdleMode : public Mode {
    using Controls = dhe::curve_sequencer::CurveSequencerControls<N>;

  public:
    IdleMode(std::vector<Param> &params) : params{params} {}

    auto isTerminal() const -> bool override { return true; }

    auto execute(dhe::Latch const &runLatch, dhe::Latch const &gateLatch, int step, float /*sampleTime*/) const
        -> Successor override {
      if (runLatch.isFall()) {
        return {ModeId::Paused, step};
      }
      if (gateLatch.isRise()) {
        return {ModeId::Advancing, startStep()};
      }
      return {ModeId::Idle, step};
    };

  private:
    auto startStep() const -> int { return static_cast<int>(valueOf(params[Controls::StartKnob])); }

    std::vector<Param> &params;
  };
}; // namespace curve_sequencer
} // namespace dhe
