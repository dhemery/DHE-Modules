#include <engine.hpp>

#include "module.hpp"
#include "util/range.hpp"

namespace DHE {

auto Module::gate_button(int button) const -> float {
  return param(button) > 0.5f ? 10.f : 0.f;
};

auto Module::knob(int rotation) const -> std::function<float()> {
  return [this, rotation] { return param(rotation); };
}

auto Module::knob(int rotation, int cv) const -> std::function<float()> {
  return [this, rotation, cv]() -> float {
    return param(rotation) + input(cv) / 10.f;
  };
}

auto Module::knob(int rotation, int cv, int av) const
    -> std::function<float()> {
  static constexpr auto av_range = Range{-1.f, 1.f};

  return [this, rotation, cv, av]() -> float {
    auto modulation_gain = av_range.scale(param(av));
    auto modulation = modulation_gain * input(cv) / 10.f;
    return param(rotation) + modulation;
  };
}

} // namespace DHE
