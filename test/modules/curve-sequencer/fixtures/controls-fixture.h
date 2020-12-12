#include "modules/curve-sequencer/curve-sequencer-controls.h"

#include "helpers/rack-controls.h"
#include <dheunit/test.h>
#include <functional>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

static auto constexpr step_count = 8;

using Controls = dhe::curve_sequencer::CurveSequencerControls<
    std::vector<test::fake::Port>, std::vector<test::fake::Port>,
    std::vector<test::fake::Param>, std::vector<test::fake::Light>, step_count>;
using Param = Controls::ParameterIds;
using Input = Controls::InputIds;
using Output = Controls::OutputIds;
using Light = Controls::LightIds;

struct Module {
  std::vector<test::fake::Port> inputs_{Input::InputCount};
  std::vector<test::fake::Port> outputs_{Output::OutputCount};
  std::vector<test::fake::Param> params_{Param::ParameterCount};
  std::vector<test::fake::Light> lights_{Light::LightCount};
};

template <typename Run> static inline auto test(Run run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Controls controls{module.inputs_, module.outputs_, module.params_,
                      module.lights_};
    run(t, module, controls);
  };
}
} // namespace curve_sequencer
} // namespace test
