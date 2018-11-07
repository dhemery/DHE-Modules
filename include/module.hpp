#pragma once

#include <engine.hpp>

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

inline std::function<float()> knob(const rack::Param &rotation) {
  return [&rotation] { return rotation.value; };
}

inline std::function<float()> knob(const rack::Param &rotation,
                                   const rack::Input &cv) {
  return
      [&rotation, &cv]() -> float { return rotation.value + cv.value / 10.f; };
}

inline std::function<float()> knob(const rack::Param &rotation,
                                   const rack::Input &cv,
                                   const rack::Param &av) {
  return [&rotation, &cv, &av]() -> float {
    auto modulation_gain = BIPOLAR_PHASE_RANGE.scale(av.value);
    auto modulation = modulation_gain * cv.value / 10.f;
    return rotation.value + modulation;
  };
}

inline std::function<const Range &()> range_switch(const rack::Param &param) {
  return [&param]() -> const Range & { return DHE::Level::range(param.value); };
}

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  float gate_button(int index) const {
    return UNIPOLAR_SIGNAL_RANGE.scale(param(index) > 0.5f);
  };

  float input(int index) const { return inputs[index].value; }

  float param(int index) const { return params[index].value; }
};

} // namespace DHE
