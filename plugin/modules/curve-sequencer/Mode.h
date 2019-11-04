#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class Mode { Idle, Advancing, Generating, Sustaining };

  struct Successor {
    Mode mode;
    int step;
  };
}; // namespace curve_sequencer
} // namespace dhe
