#pragma once

#include "dheunit/test.h"
#include "helpers/rack-controls.h"
#include "modules/curve-sequencer/control-ids.h"
#include "modules/curve-sequencer/signals.h"

#include <functional>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

static auto constexpr step_count = 8;

using Param = test::fake::Param;
using Input = test::fake::Port;
using Output = test::fake::Port;
using Light = test::fake::Light;

using ParamId = dhe::curve_sequencer::ParamIds<step_count>;
using InputId = dhe::curve_sequencer::InputIds<step_count>;
using OutputId = dhe::curve_sequencer::OutputId;
using LightId = dhe::curve_sequencer::LightIds<step_count>;

struct Module {
  std::vector<Param> params_{ParamId::Count};
  std::vector<Input> inputs_{InputId::Count};
  std::vector<Output> outputs_{OutputId::Count};
  std::vector<Light> lights_{LightId::Count};
};

using Signals =
    dhe::curve_sequencer::Signals<Param, Input, Output, Light, step_count>;

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Signals signals{module.params_, module.inputs_, module.outputs_,
                    module.lights_};
    run(t, module, signals);
  };
}
} // namespace curve_sequencer
} // namespace test
