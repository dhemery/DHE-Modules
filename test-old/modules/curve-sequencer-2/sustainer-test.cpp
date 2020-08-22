#include "helpers/latches.h"
#include "modules/curve-sequencer-2/triggers.h"

#include <doctest.h>
#include <functional>

namespace test {
namespace curve_sequencer_2 {
using dhe::curve_sequencer_2::Sustainer;
using dhe::curve_sequencer_2::TriggerMode;
using test::fallingLatch;
using test::highLatch;
using test::lowLatch;
using test::risingLatch;

namespace sustainer {
struct SustainControls {
  std::function<TriggerMode(int)> trigger_mode{
      [](int s) -> TriggerMode { throw "triggerMode " + std::to_string(s); }};
  std::function<bool(int)> advance_on_end_of_curve{
      [](int s) -> bool { throw "advanceOnEndOfCurve " + std::to_string(s); }};
};

TEST_CASE("curve_sequencer_2::Sustainer") {
  SustainControls controls{};

  Sustainer<SustainControls> sustainer{controls};

  SUBCASE("when advance on end of curve") {
    controls.advance_on_end_of_curve = [](int /**/) -> bool { return true; };

    SUBCASE("is done regardless of gate and trigger mode") {
      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateRises;
      };
      CHECK(sustainer.is_done(3, highLatch));
      CHECK(sustainer.is_done(4, lowLatch));
      CHECK(sustainer.is_done(6, risingLatch));
      CHECK(sustainer.is_done(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateFalls;
      };
      CHECK(sustainer.is_done(3, highLatch));
      CHECK(sustainer.is_done(4, lowLatch));
      CHECK(sustainer.is_done(6, risingLatch));
      CHECK(sustainer.is_done(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateChanges;
      };
      CHECK(sustainer.is_done(3, highLatch));
      CHECK(sustainer.is_done(4, lowLatch));
      CHECK(sustainer.is_done(6, risingLatch));
      CHECK(sustainer.is_done(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateIsHigh;
      };
      CHECK(sustainer.is_done(3, highLatch));
      CHECK(sustainer.is_done(4, lowLatch));
      CHECK(sustainer.is_done(6, risingLatch));
      CHECK(sustainer.is_done(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateIsLow;
      };
      CHECK(sustainer.is_done(3, highLatch));
      CHECK(sustainer.is_done(4, lowLatch));
      CHECK(sustainer.is_done(6, risingLatch));
      CHECK(sustainer.is_done(7, fallingLatch));
    }
  }

  SUBCASE("when sustain on end of curve") {
    controls.advance_on_end_of_curve = [](int /**/) -> bool { return false; };

    SUBCASE("is done iff triggered") {
      SUBCASE("by rising gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateRises;
        };
        CHECK_FALSE(sustainer.is_done(3, fallingLatch));
        CHECK_FALSE(sustainer.is_done(0, highLatch));
        CHECK_FALSE(sustainer.is_done(1, lowLatch));
        CHECK(sustainer.is_done(2, risingLatch));
      }

      SUBCASE("by falling gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateFalls;
        };
        CHECK_FALSE(sustainer.is_done(4, risingLatch));
        CHECK_FALSE(sustainer.is_done(2, highLatch));
        CHECK_FALSE(sustainer.is_done(3, lowLatch));
        CHECK(sustainer.is_done(5, fallingLatch));
      }

      SUBCASE("by changing gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateChanges;
        };
        CHECK_FALSE(sustainer.is_done(3, highLatch));
        CHECK_FALSE(sustainer.is_done(4, lowLatch));
        CHECK(sustainer.is_done(5, risingLatch));
        CHECK(sustainer.is_done(6, fallingLatch));
      }

      SUBCASE("by high gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateIsHigh;
        };
        CHECK_FALSE(sustainer.is_done(7, fallingLatch));
        CHECK_FALSE(sustainer.is_done(5, lowLatch));
        CHECK(sustainer.is_done(6, risingLatch));
        CHECK(sustainer.is_done(4, highLatch));
      }

      SUBCASE("by low gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateIsLow;
        };
        CHECK_FALSE(sustainer.is_done(7, risingLatch));
        CHECK_FALSE(sustainer.is_done(5, highLatch));
        CHECK(sustainer.is_done(0, fallingLatch));
        CHECK(sustainer.is_done(6, lowLatch));
      }
    }
  }
}
} // namespace sustainer
} // namespace curve_sequencer_2
} // namespace test
