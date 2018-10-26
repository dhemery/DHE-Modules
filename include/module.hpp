#pragma once

#include <engine.hpp>

#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  float gate_button(int index) const {
    return UNIPOLAR_SIGNAL_RANGE.scale(param(index) > 0.5f);
  };

  float input(int index) const { return inputs[index].value; }

  float modulated(int param_index, int mod_index) const {
    return param(param_index) + input(mod_index) / 10.f;
  }

  float modulated(int param_index, int mod_index,
                  int attenuverter_index) const {
    auto modulation_gain = BIPOLAR_PHASE_RANGE.scale(param(attenuverter_index));
    auto modulation = modulation_gain * input(mod_index) / 10.f;
    return param(param_index) + modulation;
  }

  float param(int index) const { return params[index].value; }
};

} // namespace DHE
