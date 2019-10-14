#include "modules/components/Latch.h"
#include "modules/curve-sequencer/StepExecutor.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::StepExecutor;

static auto constexpr defaultSampleTime = 1.F / 44100.F;

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;

class MockSequenceControls {
public:
  MOCK_METHOD(float, curvature, (int), (const));
  MOCK_METHOD(float, duration, (int), (const));
  MOCK_METHOD(int, generateMode, (int), (const));
  MOCK_METHOD(bool, isEnabled, (int), (const));
  MOCK_METHOD(float, level, (int), (const));
  MOCK_METHOD(float, output, (), (const));
  MOCK_METHOD(int, sustainMode, (int), (const));
  MOCK_METHOD(int, taperSelection, (int), (const));

  MOCK_METHOD(void, setGenerating, (int, bool) );
  MOCK_METHOD(void, setSustaining, (int, bool) );
  MOCK_METHOD(void, setOutput, (float) );
};

class StepExecutorTEst : public ::testing::Test {
protected:
  NiceMock<MockSequenceControls> controls;
  StepExecutor<MockSequenceControls> executor{controls};

  void SetUp() override {
  };
};

