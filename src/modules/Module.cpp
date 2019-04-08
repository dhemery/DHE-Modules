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

Module::Module(int param_count, int input_count, int output_count)
    : rack::Module{param_count, input_count, output_count} {}

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
} // namespace DHE