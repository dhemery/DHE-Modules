#pragma once

#include <engine.hpp>

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  float gate_button(int index) const {
    return UNIPOLAR_CV.scale(param(index) > 0.5f);
  };

  float input(int index) const {
    return inputs[index].value;
  }

  float modulated(int param_index, int mod_index) const {
    return param(param_index) + input(mod_index)/10.f;
  }

  float param(int index) const {
    return params[index].value;
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }
};

}
