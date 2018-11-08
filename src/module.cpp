#include <engine.hpp>

#include "module.hpp"
#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

float Module::gate_button(int button) const {
  return UNIPOLAR_SIGNAL_RANGE.scale(param(button) > 0.5f);
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
  return [this, rotation, cv, av]() -> float {
    auto modulation_gain = BIPOLAR_PHASE_RANGE.scale(param(av));
    auto modulation = modulation_gain * input(cv) / 10.f;
    return param(rotation) + modulation;
  };
}

std::function<const Range &()> Module::range_switch(int index) const {
  return [this, index]() -> const Range & {
    return DHE::Level::range(param(index));
  };
}

} // namespace DHE
