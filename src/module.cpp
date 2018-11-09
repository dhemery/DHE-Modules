#include <engine.hpp>

#include "module.hpp"
#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

float Module::gate_button(int button) const {
  return param(button) > 0.5f ? 10.f : 0.f;
};

std::function<float()> Module::knob(int rotation) const {
  return [this, rotation] { return param(rotation); };
}

std::function<float()> Module::knob(int rotation, int cv) const {
  return [this, rotation, cv]() -> float {
    return param(rotation) + input(cv) / 10.f;
  };
}

std::function<float()> Module::knob(int rotation, int cv, int av) const {
  static constexpr auto av_range = Range{-1.f,1.f};

  return [this, rotation, cv, av]() -> float {
    auto modulation_gain = av_range.scale(param(av));
    auto modulation = modulation_gain * input(cv) / 10.f;
    return param(rotation) + modulation;
  };
}

std::function<const Range &()> Module::range_switch(int switch_index) const {
  static constexpr auto unipolar = Range{0.f, 10.f};
  static constexpr auto bipolar = Range{-5.f, 5.f};

  return [this, switch_index]() -> const Range & {
    return param(switch_index) > 0.5f ? unipolar : bipolar;
  };
}

} // namespace DHE
