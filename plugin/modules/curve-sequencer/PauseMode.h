#pragma once

#include "Mode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  class PauseMode {
  public:
    auto execute(dhe::Latch const &runLatch) const -> Successor {
      auto const nextMode = runLatch.isRise() ? ModeId::Idle : ModeId::Paused;
      return {nextMode, 0};
    };
  };
}; // namespace curve_sequencer
} // namespace dhe
