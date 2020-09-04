#pragma once

#include "components/latch.h"
#include "interrupt-mode.h"
#include "trigger-mode.h"

namespace dhe {
namespace curve_sequencer {

template <typename Controls> class Interrupter {
public:
  Interrupter(Controls &controls) : controls_{controls} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return (controls_.interrupt_mode(step) == InterruptMode::Advance) &&
           is_triggered(controls_.trigger_mode(step), latch);
  }

private:
  Controls &controls_;
};

} // namespace curve_sequencer
} // namespace dhe
