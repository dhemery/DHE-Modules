#pragma once

#include "helpers/fake-controls.h"
#include "modules/curve-sequencer-2/TriggerMode.h"

#include <functional>
#include <string>

namespace test {
using dhe::curve_sequencer_2::TriggerMode;
namespace fake {
  struct InterruptControls {
    std::function<TriggerMode(int)> triggerMode{[](int s) -> TriggerMode { throw forbidden("triggerMode", s); }};
    std::function<bool(int)> interruptOnTrigger{[](int s) -> bool { throw forbidden("interruptOnTrigger", s); }};
  };
} // namespace fake

static inline auto triggerModeFunc(TriggerMode mode) -> std::function<TriggerMode(int)> {
  return [=](int /**/) -> TriggerMode { return mode; };
}

} // namespace test
