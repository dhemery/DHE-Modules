#pragma once

#include "anchor-mode.h"
#include "anchor-source.h"

namespace dhe {
namespace curve_sequencer {
template <typename Controls> class Anchor {
public:
  Anchor(Controls &controls) : controls_{controls} {}

  void enter(int step) {
    step_ = step;
    snapshot_ = source_voltage();
  }

  auto voltage() const -> float {
    auto const mode = controls_.mode(step_);
    return mode == AnchorMode::Track ? source_voltage() : snapshot_;
  }

private:
  Controls &controls_{};
  float snapshot_{};
  int step_{};

  auto source_voltage() const -> float {
    switch (controls_.source(step_)) {
    case AnchorSource::In:
      return controls_.input();
    case AnchorSource::Level:
      return controls_.level(step_);
    case AnchorSource::Out:
      return controls_.output();
    default:
      return 0.F;
    }
  }
};
} // namespace curve_sequencer
} // namespace dhe
