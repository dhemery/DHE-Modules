#include "modules/components/EdgeDetector.h"

#include <gmock/gmock.h>

namespace dhe {

using ::testing::AnyNumber;
using ::testing::Return;

struct Signal {
  MOCK_CONST_METHOD0(get, bool());
};

struct Runnable {
  MOCK_METHOD0(run, void());
};

struct EdgeDetectorTest : public ::testing::Test {
  Signal signal;
  Runnable onRise;
  Runnable onFall;
  EdgeDetector edgeDetector{[this]() -> bool { return signal.get(); }, [this]() { onRise.run(); },
                            [this]() { onFall.run(); }};

  void givenSignalIs(bool state) { EXPECT_CALL(signal, get()).WillOnce(Return(state)); }

  void givenStateIsHigh() {
    EXPECT_CALL(signal, get()).WillOnce(Return(true));
    EXPECT_CALL(onRise, run()).Times(AnyNumber());
    edgeDetector.step();
    EXPECT_CALL(onRise, run()).Times(0);
  }

  void givenStateIsLow() {
    EXPECT_CALL(signal, get()).WillOnce(Return(false));
    EXPECT_CALL(onFall, run()).Times(AnyNumber());
    edgeDetector.step();
    EXPECT_CALL(onFall, run()).Times(0);
  }
};

TEST_F(EdgeDetectorTest, step_callsOnRise_ifSignalIsTrue_whenStateIsLow) {
  givenStateIsLow();
  givenSignalIs(true);

  EXPECT_CALL(onRise, run());

  edgeDetector.step();
}

TEST_F(EdgeDetectorTest, step_callsOnFall_ifSignalIsFalse_whenStateIsHigh) {
  givenStateIsHigh();
  givenSignalIs(false);

  EXPECT_CALL(onFall, run());

  edgeDetector.step();
}

TEST_F(EdgeDetectorTest, step_generatesNoEvents_ifSignalIsTrue_whenStateIsHigh) {
  givenStateIsHigh();
  givenSignalIs(true);

  EXPECT_CALL(onFall, run()).Times(0);
  EXPECT_CALL(onRise, run()).Times(0);

  edgeDetector.step();
}

TEST_F(EdgeDetectorTest, step_generatesNoEvents_ifSignalIsFalse_whenStateIsLow) {
  givenStateIsLow();
  givenSignalIs(false);

  EXPECT_CALL(onFall, run()).Times(0);
  EXPECT_CALL(onRise, run()).Times(0);

  edgeDetector.step();
}

} // namespace dhe
