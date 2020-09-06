#pragma once

#include "./anchor-enums.h"
#include "./completion-modes.h"
#include "./interrupt-modes.h"
#include "helpers/rack-controls.h"
#include "modules/curve-sequencer/control-ids.h"
#include "modules/curve-sequencer/controls.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr step_count{8};

using Controls =
    dhe::curve_sequencer::Controls<fake::Port, fake::Port, fake::Param,
                                   fake::Light, step_count>;
using Param = dhe::curve_sequencer::ParamIds<step_count>;
using Input = dhe::curve_sequencer::InputIds<step_count>;
using Light = dhe::curve_sequencer::LightIds<step_count>;
using Output = dhe::curve_sequencer::OutputIds;

class Module {
public:
  Module(std::vector<fake::Port> &inputs, std::vector<fake::Port> &outputs,
         std::vector<fake::Param> &params, std::vector<fake::Light> &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  void set_param(int id, int offset, float value) {
    params_[id + offset].setValue(value);
  }
  void set_param(int id, float value) { set_param(id, 0, value); }
  void set_input(int id, int step, float voltage) {
    inputs_[id + step].setVoltage(voltage);
  }
  void set_input(int id, float voltage) { set_input(id, 0, voltage); }
  void set_output(int id, int step, float voltage) {
    outputs_[id + step].setVoltage(voltage);
  }
  void set_output(int id, float voltage) { set_output(id, 0, voltage); }
  auto get_output(int id, int step) const -> float {
    return outputs_[id + step].getVoltage();
  }
  auto get_output(int id) const -> float { return get_output(id, 0); }
  auto get_light(int id, int offset) const -> float {
    return lights_[id + offset].getBrightness();
  }
  auto get_light(int id) const -> float { return get_light(id, 0); }

private:
  std::vector<fake::Port> &inputs_;
  std::vector<fake::Port> &outputs_;
  std::vector<fake::Param> &params_;
  std::vector<fake::Light> &lights_;
};

template <typename ControlsTest>
static inline auto test(ControlsTest const &controls_test) -> TestFunc {
  return [controls_test](Tester &tester) {
    std::vector<fake::Port> inputs{Input::Count};
    std::vector<fake::Port> outputs{Output::Count};
    std::vector<fake::Param> params{Param::Count};
    std::vector<fake::Light> lights{Light::Count};

    Module module{inputs, outputs, params, lights};
    Controls controls{inputs, outputs, params, lights};
    controls_test(tester, module, controls);
  };
}
} // namespace curve_sequencer
} // namespace test
