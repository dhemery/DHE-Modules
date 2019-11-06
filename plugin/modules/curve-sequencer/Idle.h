#pragma once

#include "CurveSequencerControls.h"
#include "SequenceMode.h"
#include "SequencerState.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Param.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Param;

  template <int N> class Idle {
    using Controls = dhe::curve_sequencer::CurveSequencerControls<N>;

  public:
    Idle(std::vector<Param> &params) : params{params} {}

    auto execute(dhe::Latch const &gateLatch) const -> SequencerState {
      if (gateLatch.isRise()) {
        return {SequenceMode::Advancing, startStep()};
      }
      return {SequenceMode::Idle, 0};
    };

  private:
    auto startStep() const -> int { return static_cast<int>(valueOf(params[Controls::StartKnob])); }

    std::vector<Param> &params;
  };
}; // namespace curve_sequencer
} // namespace dhe
