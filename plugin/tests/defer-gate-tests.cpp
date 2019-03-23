
#include <bandit/bandit.h>
#include "util/stage-components.h"

using namespace snowhouse;
using namespace bandit;

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

go_bandit([]() { // NOLINT(cert-err58-cpp)
  describe("DeferGate", [&]() {
    auto module = MockModule{};
    module.the_defer_gate_in = 0.f;
    auto defer_gate = DHE::DeferGate<MockModule>{&module};

    defer_gate.step();
    
    describe("when defer_gate_in transitions above threshold", [&](){
      module.the_defer_gate_in = 0.5f;

      it("calls on_defer_gate_rise", [&]() {
        defer_gate.step();
        AssertThat(module.on_defer_gate_rise_called, IsTrue());
      });
    });
  });
});
