#include "helpers/latches.h"
#include "modules/curve-sequencer/triggers.h"

#include <doctest.h>
#include <functional>

namespace test {
namespace curve_sequencer_2 {
using dhe::curve_sequencer_2::Interrupter;
using dhe::curve_sequencer_2::TriggerMode;
using test::fallingLatch;
using test::highLatch;
using test::lowLatch;
using test::risingLatch;

namespace sustainer {
struct InterruptControls {
  std::function<TriggerMode(int)> trigger_mode{
      [](int s) -> TriggerMode { throw "triggerMode " + std::to_string(s); }};
  std::function<bool(int)> interrupt_on_trigger{
      [](int s) -> bool { throw "interruptOnTrigger " + std::to_string(s); }};
};

TEST_CASE("curve_sequencer_2::Interrupter") {
  InterruptControls controls{};

  Interrupter<InterruptControls> interrupter{controls};

  SUBCASE("when interrupt on trigger") {
    controls.interrupt_on_trigger = [](int /*unused*/) -> bool { return true; };

    SUBCASE("is interrupted iff triggered") {
      SUBCASE("by rising gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateRises;
        };
        CHECK(interrupter.is_interrupted(2, risingLatch));
        CHECK_FALSE(interrupter.is_interrupted(3, fallingLatch));
        CHECK_FALSE(interrupter.is_interrupted(0, highLatch));
        CHECK_FALSE(interrupter.is_interrupted(1, lowLatch));
      }

      SUBCASE("by falling gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateFalls;
        };
        CHECK(interrupter.is_interrupted(5, fallingLatch));
        CHECK_FALSE(interrupter.is_interrupted(4, risingLatch));
        CHECK_FALSE(interrupter.is_interrupted(2, highLatch));
        CHECK_FALSE(interrupter.is_interrupted(3, lowLatch));
      }

      SUBCASE("by changing gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateChanges;
        };
        CHECK(interrupter.is_interrupted(5, risingLatch));
        CHECK(interrupter.is_interrupted(6, fallingLatch));
        CHECK_FALSE(interrupter.is_interrupted(3, highLatch));
        CHECK_FALSE(interrupter.is_interrupted(4, lowLatch));
      }

      SUBCASE("by high gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateIsHigh;
        };
        CHECK(interrupter.is_interrupted(6, risingLatch));
        CHECK(interrupter.is_interrupted(4, highLatch));
        CHECK_FALSE(interrupter.is_interrupted(7, fallingLatch));
        CHECK_FALSE(interrupter.is_interrupted(5, lowLatch));
      }

      SUBCASE("by low gate") {
        controls.trigger_mode = [](int /**/) -> TriggerMode {
          return TriggerMode::GateIsLow;
        };
        CHECK(interrupter.is_interrupted(0, fallingLatch));
        CHECK(interrupter.is_interrupted(6, lowLatch));
        CHECK_FALSE(interrupter.is_interrupted(7, risingLatch));
        CHECK_FALSE(interrupter.is_interrupted(5, highLatch));
      }
    }
  }

  SUBCASE("when not interrupt on trigger") {
    controls.interrupt_on_trigger = [](int /*unused*/) -> bool { return false; };

    SUBCASE("is not interrupted regardless of gate and trigger mode") {
      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateRises;
      };
      CHECK_FALSE(interrupter.is_interrupted(3, highLatch));
      CHECK_FALSE(interrupter.is_interrupted(4, lowLatch));
      CHECK_FALSE(interrupter.is_interrupted(6, risingLatch));
      CHECK_FALSE(interrupter.is_interrupted(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateFalls;
      };
      CHECK_FALSE(interrupter.is_interrupted(3, highLatch));
      CHECK_FALSE(interrupter.is_interrupted(4, lowLatch));
      CHECK_FALSE(interrupter.is_interrupted(6, risingLatch));
      CHECK_FALSE(interrupter.is_interrupted(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateChanges;
      };
      CHECK_FALSE(interrupter.is_interrupted(3, highLatch));
      CHECK_FALSE(interrupter.is_interrupted(4, lowLatch));
      CHECK_FALSE(interrupter.is_interrupted(6, risingLatch));
      CHECK_FALSE(interrupter.is_interrupted(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateIsHigh;
      };
      CHECK_FALSE(interrupter.is_interrupted(3, highLatch));
      CHECK_FALSE(interrupter.is_interrupted(4, lowLatch));
      CHECK_FALSE(interrupter.is_interrupted(6, risingLatch));
      CHECK_FALSE(interrupter.is_interrupted(7, fallingLatch));

      controls.trigger_mode = [](int /**/) -> TriggerMode {
        return TriggerMode::GateIsLow;
      };
      CHECK_FALSE(interrupter.is_interrupted(3, highLatch));
      CHECK_FALSE(interrupter.is_interrupted(4, lowLatch));
      CHECK_FALSE(interrupter.is_interrupted(6, risingLatch));
      CHECK_FALSE(interrupter.is_interrupted(7, fallingLatch));
    }
  }
}
} // namespace sustainer
} // namespace curve_sequencer_2
} // namespace test
