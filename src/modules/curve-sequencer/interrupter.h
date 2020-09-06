#pragma once

#include "components/latch.h"
#include "interrupt-mode.h"
#include "trigger-mode.h"

namespace dhe {
namespace curve_sequencer {

template <typename Module> class Interrupter {
public:
  Interrupter(Module &module) : module_{module} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return (module_.interrupt_mode(step) == InterruptMode::Advance) &&
           is_triggered(module_.trigger_mode(step), latch);
  }

private:
  Module &module_;
};

} // namespace curve_sequencer
} // namespace dhe
