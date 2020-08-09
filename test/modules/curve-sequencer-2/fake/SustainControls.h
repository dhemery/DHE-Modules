#pragma once

#include "helpers.h"
#include "modules/curve-sequencer-2/TriggerMode.h"

#include <functional>
#include <string>

namespace dhe {
namespace test {
  namespace fake {
    namespace curve_sequencer_2 {
      using dhe::curve_sequencer_2::TriggerMode;
      struct SustainControls {
        std::function<TriggerMode(int)> triggerMode{[](int s) -> TriggerMode { throw forbidden("triggerMode", s); }};
        std::function<bool(int)> advanceOnEndOfCurve{[](int s) -> bool { throw forbidden("advanceOnEndOfCurve", s); }};
      };
    } // namespace curve_sequencer_2
  }   // namespace fake
} // namespace test
} // namespace dhe
