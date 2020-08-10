#pragma once

#include "helpers/fake-controls.h"
#include "modules/curve-sequencer-2/TriggerMode.h"

#include <functional>
#include <string>

namespace test {
namespace fake {
  using dhe::curve_sequencer_2::TriggerMode;
  struct InterruptControls {
    std::function<TriggerMode(int)> triggerMode{[](int s) -> TriggerMode { throw forbidden("triggerMode", s); }};
    std::function<bool(int)> interruptOnTrigger{[](int s) -> bool { throw forbidden("interruptOnTrigger", s); }};
  };
} // namespace fake
} // namespace test
