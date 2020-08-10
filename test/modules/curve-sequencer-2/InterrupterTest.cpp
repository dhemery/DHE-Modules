#include "modules/curve-sequencer-2/Interrupter.h"

#include "helpers/latches.h"

#include <doctest.h>

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
      std::function<TriggerMode(int)> triggerMode{
          [](int s) -> TriggerMode { throw "triggerMode " + std::to_string(s); }};
      std::function<bool(int)> interruptOnTrigger{
          [](int s) -> bool { throw "interruptOnTrigger " + std::to_string(s); }};
    };

    TEST_CASE("curve_sequencer_2::Interrupter") {
      InterruptControls controls{};

      Interrupter<InterruptControls> interrupter{controls};

      SUBCASE("when interrupt on trigger") {
        controls.interruptOnTrigger = [](int /*unused*/) -> bool { return true; };

        SUBCASE("is interrupted iff triggered") {
          SUBCASE("by rising gate") {
            controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateRises; };
            CHECK(interrupter.isInterrupted(2, risingLatch));
            CHECK_FALSE(interrupter.isInterrupted(3, fallingLatch));
            CHECK_FALSE(interrupter.isInterrupted(0, highLatch));
            CHECK_FALSE(interrupter.isInterrupted(1, lowLatch));
          }

          SUBCASE("by falling gate") {
            controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateFalls; };
            CHECK(interrupter.isInterrupted(5, fallingLatch));
            CHECK_FALSE(interrupter.isInterrupted(4, risingLatch));
            CHECK_FALSE(interrupter.isInterrupted(2, highLatch));
            CHECK_FALSE(interrupter.isInterrupted(3, lowLatch));
          }

          SUBCASE("by changing gate") {
            controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateChanges; };
            CHECK(interrupter.isInterrupted(5, risingLatch));
            CHECK(interrupter.isInterrupted(6, fallingLatch));
            CHECK_FALSE(interrupter.isInterrupted(3, highLatch));
            CHECK_FALSE(interrupter.isInterrupted(4, lowLatch));
          }

          SUBCASE("by high gate") {
            controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateIsHigh; };
            CHECK(interrupter.isInterrupted(6, risingLatch));
            CHECK(interrupter.isInterrupted(4, highLatch));
            CHECK_FALSE(interrupter.isInterrupted(7, fallingLatch));
            CHECK_FALSE(interrupter.isInterrupted(5, lowLatch));
          }

          SUBCASE("by low gate") {
            controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateIsLow; };
            CHECK(interrupter.isInterrupted(0, fallingLatch));
            CHECK(interrupter.isInterrupted(6, lowLatch));
            CHECK_FALSE(interrupter.isInterrupted(7, risingLatch));
            CHECK_FALSE(interrupter.isInterrupted(5, highLatch));
          }
        }
      }

      SUBCASE("when not interrupt on trigger") {
        controls.interruptOnTrigger = [](int /*unused*/) -> bool { return false; };

        SUBCASE("is not interrupted regardless of gate and trigger mode") {
          controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateRises; };
          CHECK_FALSE(interrupter.isInterrupted(3, highLatch));
          CHECK_FALSE(interrupter.isInterrupted(4, lowLatch));
          CHECK_FALSE(interrupter.isInterrupted(6, risingLatch));
          CHECK_FALSE(interrupter.isInterrupted(7, fallingLatch));

          controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateFalls; };
          CHECK_FALSE(interrupter.isInterrupted(3, highLatch));
          CHECK_FALSE(interrupter.isInterrupted(4, lowLatch));
          CHECK_FALSE(interrupter.isInterrupted(6, risingLatch));
          CHECK_FALSE(interrupter.isInterrupted(7, fallingLatch));

          controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateChanges; };
          CHECK_FALSE(interrupter.isInterrupted(3, highLatch));
          CHECK_FALSE(interrupter.isInterrupted(4, lowLatch));
          CHECK_FALSE(interrupter.isInterrupted(6, risingLatch));
          CHECK_FALSE(interrupter.isInterrupted(7, fallingLatch));

          controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateIsHigh; };
          CHECK_FALSE(interrupter.isInterrupted(3, highLatch));
          CHECK_FALSE(interrupter.isInterrupted(4, lowLatch));
          CHECK_FALSE(interrupter.isInterrupted(6, risingLatch));
          CHECK_FALSE(interrupter.isInterrupted(7, fallingLatch));

          controls.triggerMode = [](int /**/) -> TriggerMode { return TriggerMode::GateIsLow; };
          CHECK_FALSE(interrupter.isInterrupted(3, highLatch));
          CHECK_FALSE(interrupter.isInterrupted(4, lowLatch));
          CHECK_FALSE(interrupter.isInterrupted(6, risingLatch));
          CHECK_FALSE(interrupter.isInterrupted(7, fallingLatch));
        }
      }
    }
  } // namespace sustainer
} // namespace curve_sequencer_2
} // namespace test