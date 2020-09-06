#pragma once

#include "completion-mode.h"
#include "components/latch.h"
#include "trigger-mode.h"
#include <array>

namespace dhe {
namespace curve_sequencer {

template <typename Module> class Sustainer {
public:
  Sustainer(Module &module) : module_{module} {}

  auto is_done(int step, Latch const &latch) -> bool {

    return (module_.completion_mode(step) == CompletionMode::Advance) ||
           is_triggered(module_.trigger_mode(step), latch);
  }

private:
  Module &module_;
};

} // namespace curve_sequencer
} // namespace dhe
