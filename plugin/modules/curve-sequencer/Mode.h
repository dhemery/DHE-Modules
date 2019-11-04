#pragma once

#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  enum class ModeId { Paused, Idle, Advancing, Generating, Sustaining };

  struct Successor {
    ModeId mode;
    int step;
  };
}; // namespace curve_sequencer
} // namespace dhe
