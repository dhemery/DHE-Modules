#include "modules/Module.h"
#include "util/range.h"

namespace DHE {

constexpr auto av_range = Range{-1.f, 1.f};

auto cv_offset(float bipolar_voltage) -> float {
  static constexpr auto cv_to_offset = 0.1f;
  return bipolar_voltage * cv_to_offset;
}

auto av_multiplier(float av_amount) -> float {
  return av_range.scale(av_amount);
}

auto Module::modulated(int knob_param, int cv_input) const -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  return rotation + cv_offset(cv);
}

auto Module::modulated(int knob_param, int cv_input, int av_param) const
    -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  auto av = params[av_param].value;
  return rotation + av_multiplier(av) * cv_offset(cv);
}

void Module::configKnob(int index, const std::string& name) {
    configParam(index, 0.f, 1.f, 0.5f, name);
}

void Module::configCvGain(int index, const std::string &target) {
    configParam(index, 0.f, 1.f, 0.5f, target + " CV Gain", "%", 0.f, 200.f, -100.f);
}

void Module::configGain(int index, const std::string& target) {
    configParam(index, 0.f, 1.f, 0.5f, target + " Gain", "%", 0.f, 200.f);
}

void Module::configSignalRange(int index, const std::string& target, bool uni) {
    const auto initial_value = uni ? 1.f : 0.f;
    configParam(index, 0.f, 1.f, initial_value, target + " Range");
}
} // namespace DHE
