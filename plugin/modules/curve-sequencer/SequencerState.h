#pragma once

#include "SequenceMode.h"

namespace dhe {
namespace curve_sequencer {
  struct SequencerState {
    SequenceMode mode;
    int step;
  };
} // namespace curve_sequencer
} // namespace dhe