#include <modules/Tapers.h>

#include <util/rotation.h>
#include <util/sigmoid.h>
#include <util/signal.h>

namespace DHE {

Tapers::Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Tapers::step() {
  outputs[OUT_1].value = taper(level1(), is_uni_1(), curvature1(), is_s_1());
  outputs[OUT_2].value = taper(level2(), is_uni_2(), curvature2(), is_s_2());
}

auto Tapers::curvature(Tapers::ParameterIds knob, Tapers::InputIds cv,
                       Tapers::ParameterIds av) const -> float {
  auto curvature = modulated(knob, cv, av);
  return Sigmoid::curvature(curvature);
}

auto Tapers::curvature1() const -> float {
  return curvature(CURVE_1_KNOB, CURVE_1_CV, CURVE_1_AV);
}

auto Tapers::curvature2() const -> float {
  return curvature(CURVE_2_KNOB, CURVE_2_CV, CURVE_2_AV);
}

auto Tapers::is_uni_1() const -> bool {
  return params[RANGE_1_SWITCH].value > 0.5f;
}

auto Tapers::is_uni_2() const -> bool {
  return params[RANGE_2_SWITCH].value > 0.5f;
}

auto Tapers::is_s_1() const -> bool {
  return params[SHAPE_1_SWITCH].value > 0.5f;
}

auto Tapers::is_s_2() const -> bool {
  return params[SHAPE_2_SWITCH].value > 0.5f;
}

auto Tapers::level1() const -> float {
  return modulated(LEVEL_1_KNOB, LEVEL_1_CV, LEVEL_1_AV);
}

auto Tapers::level2() const -> float {
  return modulated(LEVEL_2_KNOB, LEVEL_2_CV, LEVEL_2_AV);
}

auto Tapers::modulated(Tapers::ParameterIds knob_param,
                       Tapers::InputIds cv_input,
                       Tapers::ParameterIds av_param) const -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  auto av = params[av_param].value;
  return Rotation::modulated(rotation, cv, av);
}

auto Tapers::taper(float level, bool is_uni, float curve, bool is_s) const
    -> float {
  auto tapered = Sigmoid::taper(level, curve, is_s);
  return Signal::range(is_uni).scale(tapered);
}

} // namespace DHE
