#include "modules/curve-sequencer-2/Sustainer.h"

#include "fake/SustainControls.h"
#include "helpers/latches.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
  using dhe::curve_sequencer_2::Sustainer;
  using dhe::curve_sequencer_2::TriggerMode;
  using test::fallingLatch;
  using test::highLatch;
  using test::lowLatch;
  using test::risingLatch;
  using test::fake::funcReturning;
  using test::fake::SustainControls;

  namespace sustainer {

    TEST_CASE("curve_sequencer_2::Sustainer") {
      SustainControls controls{};

      Sustainer<SustainControls> sustainer{controls};

      SUBCASE("when advance on end of curve") {
        controls.advanceOnEndOfCurve = funcReturning<int>(true);

        SUBCASE("is done regardless of gate and trigger mode") {
          controls.triggerMode = funcReturning<int>(TriggerMode::GateRises);
          CHECK(sustainer.isDone(3, highLatch));
          CHECK(sustainer.isDone(4, lowLatch));
          CHECK(sustainer.isDone(6, risingLatch));
          CHECK(sustainer.isDone(7, fallingLatch));

          controls.triggerMode = funcReturning<int>(TriggerMode::GateFalls);
          CHECK(sustainer.isDone(3, highLatch));
          CHECK(sustainer.isDone(4, lowLatch));
          CHECK(sustainer.isDone(6, risingLatch));
          CHECK(sustainer.isDone(7, fallingLatch));

          controls.triggerMode = funcReturning<int>(TriggerMode::GateChanges);
          CHECK(sustainer.isDone(3, highLatch));
          CHECK(sustainer.isDone(4, lowLatch));
          CHECK(sustainer.isDone(6, risingLatch));
          CHECK(sustainer.isDone(7, fallingLatch));

          controls.triggerMode = funcReturning<int>(TriggerMode::GateIsHigh);
          CHECK(sustainer.isDone(3, highLatch));
          CHECK(sustainer.isDone(4, lowLatch));
          CHECK(sustainer.isDone(6, risingLatch));
          CHECK(sustainer.isDone(7, fallingLatch));

          controls.triggerMode = funcReturning<int>(TriggerMode::GateIsLow);
          CHECK(sustainer.isDone(3, highLatch));
          CHECK(sustainer.isDone(4, lowLatch));
          CHECK(sustainer.isDone(6, risingLatch));
          CHECK(sustainer.isDone(7, fallingLatch));
        }
      }

      SUBCASE("when sustain on end of curve") {
        controls.advanceOnEndOfCurve = funcReturning<int>(false);

        SUBCASE("is done iff triggered") {
          SUBCASE("by rising gate") {
            controls.triggerMode = funcReturning<int>(TriggerMode::GateRises);
            CHECK_FALSE(sustainer.isDone(3, fallingLatch));
            CHECK_FALSE(sustainer.isDone(0, highLatch));
            CHECK_FALSE(sustainer.isDone(1, lowLatch));
            CHECK(sustainer.isDone(2, risingLatch));
          }

          SUBCASE("by falling gate") {
            controls.triggerMode = funcReturning<int>(TriggerMode::GateFalls);
            CHECK_FALSE(sustainer.isDone(4, risingLatch));
            CHECK_FALSE(sustainer.isDone(2, highLatch));
            CHECK_FALSE(sustainer.isDone(3, lowLatch));
            CHECK(sustainer.isDone(5, fallingLatch));
          }

          SUBCASE("by changing gate") {
            controls.triggerMode = funcReturning<int>(TriggerMode::GateChanges);
            CHECK_FALSE(sustainer.isDone(3, highLatch));
            CHECK_FALSE(sustainer.isDone(4, lowLatch));
            CHECK(sustainer.isDone(5, risingLatch));
            CHECK(sustainer.isDone(6, fallingLatch));
          }

          SUBCASE("by high gate") {
            controls.triggerMode = funcReturning<int>(TriggerMode::GateIsHigh);
            CHECK_FALSE(sustainer.isDone(7, fallingLatch));
            CHECK_FALSE(sustainer.isDone(5, lowLatch));
            CHECK(sustainer.isDone(6, risingLatch));
            CHECK(sustainer.isDone(4, highLatch));
          }

          SUBCASE("by low gate") {
            controls.triggerMode = funcReturning<int>(TriggerMode::GateIsLow);
            CHECK_FALSE(sustainer.isDone(7, risingLatch));
            CHECK_FALSE(sustainer.isDone(5, highLatch));
            CHECK(sustainer.isDone(0, fallingLatch));
            CHECK(sustainer.isDone(6, lowLatch));
          }
        }
      }
    }
  } // namespace sustainer
} // namespace curve_sequencer_2
} // namespace test
