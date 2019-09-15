
#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace dhe {

constexpr float sampleTime = 1.F / 44000.F;

using ::testing::Return;
using ::testing::StrictMock;

struct CurveSequencerModule {
  virtual auto isRunning() -> bool = 0;
  virtual void send(float voltage) = 0;
};

struct FakeCurveSequencer : public CurveSequencerModule {
  MOCK_METHOD(bool, isRunning, (), ());
  MOCK_METHOD(void, send, (float), ());
};

class CurveSequencerEngine {
public:
  explicit CurveSequencerEngine(CurveSequencerModule &module, int numberOfStages) : module{module} {}

  void process(float sampleTime) { module.isRunning(); }

private:
  CurveSequencerModule &module;
};

struct IfNotRunning : public ::testing::Test {
  StrictMock<FakeCurveSequencer> sequencer; // Fails if unspecified function called
  CurveSequencerEngine engine{sequencer, 4};

  void SetUp() override { EXPECT_CALL(sequencer, isRunning()).WillRepeatedly(Return(false)); }
};

TEST_F(IfNotRunning, doesNothing) { engine.process(sampleTime); }

} // namespace dhegs
