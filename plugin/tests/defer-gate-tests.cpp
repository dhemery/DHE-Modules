
#include <gmock/gmock.h>
#include "stage/components/defer-gate.h"

namespace {

using ::testing::Expectation;
using ::testing::NiceMock;
using ::testing::Return;

struct MockModule {
  MOCK_CONST_METHOD0(defer_gate_in, float());
  MOCK_METHOD0(on_defer_gate_fall, void());
  MOCK_METHOD0(on_defer_gate_rise, void());
};

TEST(DeferGateTest, WhenDeferSignalRises_CallsOnDeferGateRise) {
  MockModule module;
  auto defer_gate = DHE::DeferGate<MockModule>{&module};

  Expectation gate_rises = EXPECT_CALL(module, defer_gate_in())
      .WillOnce(Return(0.5f));

  EXPECT_CALL(module, on_defer_gate_rise())
      .After(gate_rises);

  defer_gate.step();
}

TEST(DeferGateTest, WhenDeferSignalFalls_CallsOnDeferGateFall) {
  NiceMock<MockModule> module;
  auto defer_gate = DHE::DeferGate<MockModule>{&module};

  Expectation gate_falls = EXPECT_CALL(module, defer_gate_in())
      .WillOnce(Return(0.5f)) // rise
      .WillOnce(Return(0.f)); // fall

  EXPECT_CALL(module, on_defer_gate_fall())
      .After(gate_falls);

  defer_gate.step();
  defer_gate.step();
}
}
