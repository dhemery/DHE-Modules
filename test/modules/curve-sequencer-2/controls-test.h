#pragma once

#include "helpers/rack-controls.h"
#include "modules/curve-sequencer-2/controls.h"

#include <dheunit/test.h>

namespace test {
namespace curve_sequencer_2 {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr step_count{8};

using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, step_count>;

class Module {
public:
  Module(std::vector<fake::Port> &inputs, std::vector<fake::Port> &outputs,
         std::vector<fake::Param> &params, std::vector<fake::Light> &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  void set(Controls::ParamIds id, int offset, float value) {
    params_[id + offset].setValue(value);
  }
  void set(Controls::ParamIds id, float value) { set(id, 0, value); }
  void set(Controls::InputIds id, int step, float voltage) {
    inputs_[id + step].setVoltage(voltage);
  }
  void set(Controls::InputIds id, float voltage) { set(id, 0, voltage); }
  void set(Controls::OutputIds id, int step, float voltage) {
    outputs_[id + step].setVoltage(voltage);
  }
  void set(Controls::OutputIds id, float voltage) { set(id, 0, voltage); }
  auto get(Controls::OutputIds id, int step) const -> float {
    return outputs_[id + step].getVoltage();
  }
  auto get(Controls::OutputIds id) const -> float { return get(id, 0); }

private:
  std::vector<fake::Port> &inputs_;
  std::vector<fake::Port> &outputs_;
  std::vector<fake::Param> &params_;
  std::vector<fake::Light> &lights_;
};

template <typename ControlsTest>
static inline auto test(ControlsTest const &controls_test) -> TestFunc {
  return [controls_test](Tester &tester) {
    std::vector<fake::Port> inputs{Controls::InputCount};
    std::vector<fake::Port> outputs{Controls::OutputCount};
    std::vector<fake::Param> params{Controls::ParamCount};
    std::vector<fake::Light> lights{Controls::LightCount};

    Module module{inputs, outputs, params, lights};
    Controls controls{inputs, outputs, params, lights};
    controls_test(tester, module, controls);
  };
}
} // namespace curve_sequencer_2
} // namespace test
