#pragma once

#include "modules/curve-sequencer/sequence-controller.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::Latch;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;

struct Controls {};
struct StepSelector {};
struct StepController {};

using SequenceController =
    dhe::curve_sequencer::SequenceController<Controls, StepSelector,
                                             StepController>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Controls controls{};
    StepController step_controller{};
    StepSelector step_selector{};
    SequenceController sequence_controller{controls, step_selector,
                                           step_controller};
    run(t, controls, step_selector, step_controller, sequence_controller);
  };
}
} // namespace curve_sequencer
} // namespace test
