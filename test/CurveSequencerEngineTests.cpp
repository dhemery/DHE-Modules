
#include "modules/CurveSequencer.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace dhe {

class CurveSequencerStepController {
public:
  virtual auto isAvailable(int step) -> bool = 0;
  virtual void start(int step) = 0;
};

class CurveSequencerEngine {
public:
  explicit CurveSequencerEngine(CurveSequencer &sequenceController, CurveSequencerStepController &stepController) :
      module{sequenceController},
      stepController{stepController} {}

  void process(float sampleTime) {
    if (!module.isRunning()) {
      return;
    }
    stepController.start(0);
  }

private:
  CurveSequencer &module;
  CurveSequencerStepController &stepController;
};

constexpr float sampleTime = 1.F / 44000.F;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Test;

struct FakeCurveSequencer : public CurveSequencer {
  MOCK_METHOD(int, gate, (), ());
  MOCK_METHOD(bool, isRunning, (), ());
  MOCK_METHOD(void, send, (float), ());
  MOCK_METHOD(int, startStep, (), ());
};

struct FakeCurveSequencerStepController : public CurveSequencerStepController {
  MOCK_METHOD(bool, isAvailable, (int), ());
  MOCK_METHOD(void, start, (int), ());
};

struct WhileNotRunning : public Test {
  StrictMock<FakeCurveSequencer> sequenceController;
  StrictMock<FakeCurveSequencerStepController> stepController;
  CurveSequencerEngine engine{sequenceController, stepController};

  void SetUp() override { EXPECT_CALL(sequenceController, isRunning()).WillRepeatedly(Return(false)); }
};

struct WhileIdle : public Test {
  NiceMock<FakeCurveSequencer> sequenceController;
  NiceMock<FakeCurveSequencerStepController> stepController;
  CurveSequencerEngine engine{sequenceController, stepController};

  void SetUp() override { ON_CALL(sequenceController, isRunning()).WillByDefault(Return(true)); }
};

TEST_F(WhileNotRunning, doesNothing) { engine.process(sampleTime); }

TEST_F(WhileIdle, gateRiseStartsFirstAvailableStep) {
  auto firstAvailableStep = 0;
  ON_CALL(sequenceController, gate()).WillByDefault(Return(3));
  ON_CALL(sequenceController, startStep()).WillByDefault(Return(firstAvailableStep));
  ON_CALL(stepController, isAvailable(firstAvailableStep)).WillByDefault(Return(true));

  EXPECT_CALL(stepController, start(firstAvailableStep));

  engine.process(sampleTime);
}

} // namespace dhe
