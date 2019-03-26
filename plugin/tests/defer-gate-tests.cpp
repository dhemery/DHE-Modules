#include <gmock/gmock.h>
#include "envelopes/components/defer-gate.h"

namespace {

using ::testing::AnyNumber;
using ::testing::AtMost;
using ::testing::Expectation;
using ::testing::NiceMock;
using ::testing::Return;

struct Module {
  MOCK_CONST_METHOD0(defer_gate_in, float());
};

struct Runnable {
  MOCK_METHOD0(run, void());
};

struct DeferGateTest : public ::testing::Test {
  Module source;
  Runnable on_rise;
  Runnable on_fall;
  DHE::DeferGate<Module> defer_gate{&source, [this]() {on_rise.run();}, [this]() {on_fall.run();}  };

  void given_defer_signal(bool state) {
    EXPECT_CALL(source, defer_gate_in())
        .WillOnce(Return(state));
  }

  void given_gate_is_high() {
    EXPECT_CALL(source, defer_gate_in())
        .WillOnce(Return(true));
    EXPECT_CALL(on_rise, run())
        .Times(AnyNumber());
    defer_gate.step();
    EXPECT_CALL(on_rise, run())
        .Times(0);
  }

  void given_gate_is_low() {
    EXPECT_CALL(source, defer_gate_in())
        .WillOnce(Return(false));
    EXPECT_CALL(on_fall, run())
        .Times(AnyNumber());
    defer_gate.step();
    EXPECT_CALL(on_fall, run())
        .Times(0);
  }
};

TEST_F(DeferGateTest, step_callsOnDeferGateRise_ifDeferSignalIsTrue_whenGateIsLow) {
  given_gate_is_low();
  given_defer_signal(true);

  EXPECT_CALL(on_rise, run());

  defer_gate.step();
}

TEST_F(DeferGateTest, step_callsOnDeferGateFall_ifDeferSignalIsFalse_whenGateIsHigh) {
  given_gate_is_high();
  given_defer_signal(false);

  EXPECT_CALL(on_fall, run());

  defer_gate.step();
}

TEST_F(DeferGateTest, step_generatesNoEvents_ifDeferSignalIsTrue_whenGateIsHigh) {
  given_gate_is_high();
  given_defer_signal(true);

  EXPECT_CALL(on_fall, run()).Times(0);
  EXPECT_CALL(on_rise, run()).Times(0);

  defer_gate.step();
}


TEST_F(DeferGateTest, step_generatesNoEvents_ifDeferSignalIsFalse_whenGateIsLow) {
  given_gate_is_low();
  given_defer_signal(false);

  EXPECT_CALL(on_fall, run()).Times(0);
  EXPECT_CALL(on_rise, run()).Times(0);

  defer_gate.step();
}

}
