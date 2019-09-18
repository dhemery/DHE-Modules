
#include "modules/CurveSequencer.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace dhe {

constexpr float sampleTime = 1.F / 44000.F;

using ::testing::Return;
using ::testing::StrictMock;

struct FakeCurveSequencer : public CurveSequencer {
  MOCK_METHOD(bool, isRunning, (), ());
  MOCK_METHOD(void, send, (float), ());
};

class CurveSequencerEngine {
public:
  explicit CurveSequencerEngine(CurveSequencer &module, int numberOfStages) : module{module} {}

  void process(float sampleTime) { module.isRunning(); }

private:
  CurveSequencer &module;
};

struct IfNotRunning : public ::testing::Test {
  StrictMock<FakeCurveSequencer> sequencer; // Fails if unspecified function called
  CurveSequencerEngine engine{sequencer, 4};

  void SetUp() override { EXPECT_CALL(sequencer, isRunning()).WillRepeatedly(Return(false)); }
};

TEST_F(IfNotRunning, doesNothing) { engine.process(sampleTime); }

} // namespace dhegs
