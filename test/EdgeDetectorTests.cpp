#include "modules/components/edge-detector.h"
#include "gmock/gmock.h"

namespace {

using ::testing::AnyNumber;
using ::testing::AtMost;
using ::testing::Expectation;
using ::testing::NiceMock;
using ::testing::Return;

struct Signal {
  MOCK_CONST_METHOD0(get, bool());
};

struct Runnable {
  MOCK_METHOD0(run, void());
};

struct EdgeDetectorTest : public ::testing::Test {
  Signal signal;
  Runnable on_rise;
  Runnable on_fall;
  DHE::EdgeDetector edge_detector{[this]() -> bool { return signal.get(); },
                                  [this]() { on_rise.run(); },
                                  [this]() { on_fall.run(); }};

  void given_signal_is(bool state) {
    EXPECT_CALL(signal, get()).WillOnce(Return(state));
  }

  void given_state_is_high() {
    EXPECT_CALL(signal, get()).WillOnce(Return(true));
    EXPECT_CALL(on_rise, run()).Times(AnyNumber());
    edge_detector.step();
    EXPECT_CALL(on_rise, run()).Times(0);
  }

  void given_state_is_low() {
    EXPECT_CALL(signal, get()).WillOnce(Return(false));
    EXPECT_CALL(on_fall, run()).Times(AnyNumber());
    edge_detector.step();
    EXPECT_CALL(on_fall, run()).Times(0);
  }
};

TEST_F(EdgeDetectorTest, step_callsOnRise_ifSignalIsTrue_whenStateIsLow) {
  given_state_is_low();
  given_signal_is(true);

  EXPECT_CALL(on_rise, run());

  edge_detector.step();
}

TEST_F(EdgeDetectorTest, step_callsOnFall_ifSignalIsFalse_whenStateIsHigh) {
  given_state_is_high();
  given_signal_is(false);

  EXPECT_CALL(on_fall, run());

  edge_detector.step();
}

TEST_F(EdgeDetectorTest,
       step_generatesNoEvents_ifSignalIsTrue_whenStateIsHigh) {
  given_state_is_high();
  given_signal_is(true);

  EXPECT_CALL(on_fall, run()).Times(0);
  EXPECT_CALL(on_rise, run()).Times(0);

  edge_detector.step();
}

TEST_F(EdgeDetectorTest,
       step_generatesNoEvents_ifSignalIsFalse_whenStateIsLow) {
  given_state_is_low();
  given_signal_is(false);

  EXPECT_CALL(on_fall, run()).Times(0);
  EXPECT_CALL(on_rise, run()).Times(0);

  edge_detector.step();
}

} // namespace
