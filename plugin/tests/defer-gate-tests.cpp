
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "util/stage-components.h"

namespace {

struct MockModule {
  bool on_defer_gate_rise_called;
  bool on_defer_gate_fall_called;
  float the_defer_gate_in;

  auto on_defer_gate_rise() -> void {
    on_defer_gate_rise_called = true;
  }

  auto on_defer_gate_fall() -> void {
    on_defer_gate_fall_called = true;
  }

  auto defer_gate_in() -> float {
    return the_defer_gate_in;
  }
};

TEST(DeferGateTest, WhenDeferGateRises_CallsOnDeferGateRise) {
  auto module = MockModule{};
  auto defer_gate = DHE::DeferGate<MockModule>{&module};

  defer_gate.step();

  module.the_defer_gate_in = 0.5f;

  defer_gate.step();
  EXPECT_TRUE(module.on_defer_gate_rise_called);
}
}
