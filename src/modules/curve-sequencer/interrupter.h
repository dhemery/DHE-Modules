#pragma once

#include "components/latch.h"
#include "trigger-mode.h"
#include <array>

namespace dhe {
namespace curve_sequencer {

template <typename Controls> class Interrupter {
public:
  Interrupter(Controls &controls) : controls_{controls} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return controls_.interrupt_on_trigger(step) &&
           is_triggered(controls_.trigger_mode(step), latch);
  }

private:
  Controls &controls_;
};

} // namespace curve_sequencer
} // namespace dhe
