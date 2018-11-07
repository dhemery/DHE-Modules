#pragma once

#include <engine.hpp>

#include "util/controls.hpp"
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

  std::function<float()> knob(const rack::Param &rotation) {
    return [&rotation] { return rotation.value; };
  }

  std::function<float()> knob(const rack::Param &rotation,
                              const rack::Input &cv) {
    return [&rotation, &cv]() -> float {
      return rotation.value + cv.value / 10.f;
    };
  }

  std::function<float()> knob(const rack::Param &rotation,
                              const rack::Input &cv, const rack::Param &av) {
    return [&rotation, &cv, &av]() -> float {
      auto modulation_gain = BIPOLAR_PHASE_RANGE.scale(av.value);
      auto modulation = modulation_gain * cv.value / 10.f;
      return rotation.value + modulation;
    };
  }

  std::function<const Range &()> range_switch(const rack::Param &param) {
    return
        [&param]() -> const Range & { return DHE::Level::range(param.value); };
  }
};

} // namespace DHE
