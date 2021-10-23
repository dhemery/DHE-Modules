#pragma once

#include "modules/curve-sequencer/control-ids.h"
#include "modules/curve-sequencer/signals.h"

#include "helpers/rack-controls.h"

#include "dheunit/test.h"
#include <functional>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

static auto constexpr step_count = 8;

using Signals = dhe::curve_sequencer::Signals<
    std::vector<test::fake::Port>, std::vector<test::fake::Port>,
    std::vector<test::fake::Param>, std::vector<test::fake::Light>, step_count>;
using Input = dhe::curve_sequencer::InputIds<step_count>;
using Light = dhe::curve_sequencer::LightIds<step_count>;
using Output = dhe::curve_sequencer::OutputIds;
using Param = dhe::curve_sequencer::ParamIds<step_count>;

struct Module {
  std::vector<test::fake::Port> inputs_{Input::InputCount};
  std::vector<test::fake::Port> outputs_{Output::OutputCount};
  std::vector<test::fake::Param> params_{Param::ParameterCount};
  std::vector<test::fake::Light> lights_{Light::LightCount};
};

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Signals signals{module.inputs_, module.outputs_, module.params_,
                    module.lights_};
    run(t, module, signals);
  };
}
} // namespace curve_sequencer
} // namespace test