#pragma once

#include "components/latch.h"
#include "trigger-mode.h"
#include <array>

namespace dhe {
namespace curve_sequencer {

template <typename Controls> class Sustainer {
public:
  Sustainer(Controls &controls) : controls_{controls} {}

  auto is_done(int step, Latch const &latch) -> bool {
    return controls_.completion_mode(step) ||
           is_triggered(controls_.trigger_mode(step), latch);
  }

private:
  Controls &controls_;
};

} // namespace curve_sequencer
} // namespace dhe
