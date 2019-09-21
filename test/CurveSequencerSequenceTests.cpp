#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace dhe {

namespace curve_sequencer {

  template <typename M> class Step {};

  template <typename M, typename StepT = Step<M>> class Sequence {
  public:
    explicit Sequence(M &module) : module{module} {}

    void process(float sampleTime) {
      if (!module.isRunning()) {
        return;
      }
      module.step(0).start();
    }

  private:
    M &module;
  };
} // namespace curve_sequencer

} // namespace dhe

struct FakeStep {
  MOCK_METHOD(bool, isAvailable, ());
  MOCK_METHOD(void, start, ());
};

struct FakeModule {
  MOCK_METHOD(bool, isRunning, ());
  MOCK_METHOD(bool, gate, ());
  MOCK_METHOD(int, startStep, ());
  MOCK_METHOD(FakeStep &, step, (int) );
};

using ::testing::Return;
using ::testing::ReturnRef;
using Sequence = dhe::curve_sequencer::Sequence<FakeModule, FakeStep>;

float constexpr sampleTime = 1.F / 44000.F;

struct SequenceTest : ::testing::Test {
  FakeModule module;
  Sequence sequence = Sequence{module};
};

struct WhileNotRunning : public SequenceTest {
  void SetUp() { ON_CALL(module, isRunning()).WillByDefault(Return(false)); }
};

struct WhileIdle : public SequenceTest {
  void SetUp() { ON_CALL(module, isRunning()).WillByDefault(Return(true)); }
};

TEST_F(WhileNotRunning, doesNothing) { sequence.process(sampleTime); }

TEST_F(WhileIdle, gateRiseStartsFirstAvailableStep) {
  FakeStep step0;
  auto firstAvailableStep = 0;
  ON_CALL(module, gate()).WillByDefault(Return(true));
  ON_CALL(module, startStep()).WillByDefault(Return(firstAvailableStep));
  ON_CALL(module, step(firstAvailableStep)).WillByDefault(ReturnRef(step0));
  ON_CALL(step0, isAvailable()).WillByDefault(Return(true));

  EXPECT_CALL(step0, start());

  sequence.process(sampleTime);
}
