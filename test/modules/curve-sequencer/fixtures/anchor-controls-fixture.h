#pragma once

#include "helpers/rack-controls.h"
#include "modules/curve-sequencer/anchor-controls.h"
#include "modules/curve-sequencer/control-ids.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr step_count{8};

using Controls = dhe::curve_sequencer::AnchorControls<fake::Port, fake::Port,
                                                      fake::Param, step_count>;
using Param = dhe::curve_sequencer::ParamIds<step_count>;
using Input = dhe::curve_sequencer::InputIds<step_count>;
using Output = dhe::curve_sequencer::OutputIds;

class Module {
public:
  Module(std::vector<fake::Port> &inputs, std::vector<fake::Port> &outputs,
         std::vector<fake::Param> &params)
      : inputs_{inputs}, outputs_{outputs}, params_{params} {}

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

private:
  std::vector<fake::Port> &inputs_;
  std::vector<fake::Port> &outputs_;
  std::vector<fake::Param> &params_;
};

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &tester) {
    std::vector<fake::Port> inputs{Input::Count};
    std::vector<fake::Port> outputs{Output::Count};
    std::vector<fake::Param> params{Param::Count};

    Module module{inputs, outputs, params};

    Controls controls{inputs,
                      outputs,
                      params,
                      Param::StepStartAnchorMode,
                      Param::StepStartAnchorSource,
                      Param::StepStartLevel};
    run(tester, module, controls);
  };
}
} // namespace curve_sequencer
} // namespace test
