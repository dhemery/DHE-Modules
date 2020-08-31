#pragma once

#include "helpers/rack-controls.h"
#include "modules/curve-sequencer/controls.h"
#include "types/enums.h"

#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::enum_index;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr step_count{8};

using Controls =
    dhe::curve_sequencer::Controls<fake::Port, fake::Port, fake::Param,
                                   fake::Light, step_count>;

class Module {
public:
  Module(std::vector<fake::Port> &inputs, std::vector<fake::Port> &outputs,
         std::vector<fake::Param> &params, std::vector<fake::Light> &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  void set(Controls::Param id, int offset, float value) {
    params_[enum_index(id, offset)].setValue(value);
  }
  void set(Controls::Param id, float value) { set(id, 0, value); }
  void set(Controls::Input id, int step, float voltage) {
    inputs_[enum_index(id, step)].setVoltage(voltage);
  }
  void set(Controls::Input id, float voltage) { set(id, 0, voltage); }
  void set(Controls::Output id, int step, float voltage) {
    outputs_[enum_index(id, step)].setVoltage(voltage);
  }
  void set(Controls::Output id, float voltage) { set(id, 0, voltage); }
  auto get(Controls::Output id, int step) const -> float {
    return outputs_[enum_index(id, step)].getVoltage();
  }
  auto get(Controls::Output id) const -> float { return get(id, 0); }
  auto get(Controls::Light id, int offset) const -> float {
    return lights_[enum_index(id, offset)].getBrightness();
  }
  auto get(Controls::Light id) const -> float { return get(id, 0); }

private:
  std::vector<fake::Port> &inputs_;
  std::vector<fake::Port> &outputs_;
  std::vector<fake::Param> &params_;
  std::vector<fake::Light> &lights_;
};

template <typename ControlsTest>
static inline auto test(ControlsTest const &controls_test) -> TestFunc {
  return [controls_test](Tester &tester) {
    std::vector<fake::Port> inputs{enum_index(Controls::Input::Count)};
    std::vector<fake::Port> outputs{enum_index(Controls::Output::Count)};
    std::vector<fake::Param> params{enum_index(Controls::Param::Count)};
    std::vector<fake::Light> lights{enum_index(Controls::Light::Count)};

    Module module{inputs, outputs, params, lights};
    Controls controls{inputs, outputs, params, lights};
    controls_test(tester, module, controls);
  };
}
} // namespace curve_sequencer
} // namespace test
