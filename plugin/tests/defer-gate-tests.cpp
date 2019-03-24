
#include <gmock/gmock.h>
#include "stage/components/defer-gate.h"

namespace {

using ::testing::AnyNumber;
using ::testing::AtMost;
using ::testing::Expectation;
using ::testing::NiceMock;
using ::testing::Return;

struct Source {
  MOCK_CONST_METHOD0(defer_gate_in, float());
};

struct Sink {
  MOCK_METHOD0(on_defer_gate_fall, void());
  MOCK_METHOD0(on_defer_gate_rise, void());
};

struct DeferGateTest : public ::testing::Test {
  Source source;
  Sink sink;
  DHE::DeferGate<Source,Sink> defer_gate{&source, &sink};

  void given_defer_signal(bool state) {
    EXPECT_CALL(source, defer_gate_in())
        .WillOnce(Return(state));
  }

  void given_gate_is_high() {
    EXPECT_CALL(source, defer_gate_in())
        .WillOnce(Return(true));
    EXPECT_CALL(sink, on_defer_gate_rise())
        .Times(AnyNumber());
    defer_gate.step();
    EXPECT_CALL(sink, on_defer_gate_rise())
        .Times(0);
  }

  void given_gate_is_low() {
    EXPECT_CALL(source, defer_gate_in())
        .WillOnce(Return(false));
    EXPECT_CALL(sink, on_defer_gate_fall())
        .Times(AnyNumber());
    defer_gate.step();
    EXPECT_CALL(sink, on_defer_gate_fall())
        .Times(0);
  }
};

TEST_F(DeferGateTest, step_callsOnDeferGateRise_ifDeferSignalIsTrue_whenGateIsLow) {
  given_gate_is_low();
  given_defer_signal(true);

  EXPECT_CALL(sink, on_defer_gate_rise());

  defer_gate.step();
}

TEST_F(DeferGateTest, step_callsOnDeferGateFall_ifDeferSignalIsFalse_whenGateIsHigh) {
  given_gate_is_high();
  given_defer_signal(false);

  EXPECT_CALL(sink, on_defer_gate_fall());

  defer_gate.step();
}

TEST_F(DeferGateTest, step_generatesNoEvents_ifDeferSignalIsTrue_whenGateIsHigh) {
  given_gate_is_high();
  given_defer_signal(true);

  EXPECT_CALL(sink, on_defer_gate_fall()).Times(0);
  EXPECT_CALL(sink, on_defer_gate_rise()).Times(0);

  defer_gate.step();
}


TEST_F(DeferGateTest, step_generatesNoEvents_ifDeferSignalIsFalse_whenGateIsLow) {
  given_gate_is_low();
  given_defer_signal(false);

  EXPECT_CALL(sink, on_defer_gate_fall()).Times(0);
  EXPECT_CALL(sink, on_defer_gate_rise()).Times(0);

  defer_gate.step();
}

}
