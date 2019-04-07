#include <modules/Xycloid.h>

#include <array>

#include "util/rotation.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

Xycloid::Xycloid() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Xycloid::set_musical_wobble_ratios(bool is_musical) {
  wobble_ratio_offset = is_musical ? 0.f : 1.f;
}

auto Xycloid::is_musical_wobble_ratios() const -> bool {
  return wobble_ratio_offset == 0.f;
}

void Xycloid::step() {
  auto wobble_ratio = this->wobble_ratio();
  auto wobble_phase_offset = wobble_phase_in();
  if (wobble_ratio < 0.f)
    wobble_phase_offset *= -1.f;

  auto throb_speed = this->throb_speed();
  auto wobble_speed = wobble_ratio * throb_speed;
  auto wobble_depth = this->wobble_depth();
  auto throb_depth = 1.f - wobble_depth;

  throbber.advance(throb_speed);
  wobbler.advance(wobble_speed, wobble_phase_offset);
  auto x = throb_depth * throbber.x() + wobble_depth * wobbler.x();
  auto y = throb_depth * throbber.y() + wobble_depth * wobbler.y();

  outputs[X_OUT].value = 5.f * x_gain_in() * (x + x_offset());
  outputs[Y_OUT].value = 5.f * y_gain_in() * (y + y_offset());
}

auto Xycloid::is_wobble_ratio_free() const -> bool {
  return params[WOBBLE_RATIO_FREEDOM_SWITCH].value > 0.1f;
}

auto Xycloid::modulated(const Xycloid::ParameterIds &knob_param,
                        const Xycloid::InputIds &cv_input) const -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  return Rotation::modulated(rotation, cv);
}

auto Xycloid::modulated(const Xycloid::ParameterIds &knob_param,
                        const Xycloid::InputIds &cv_input,
                        const Xycloid::ParameterIds &av_param) const -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  auto av = params[av_param].value;
  return Rotation::modulated(rotation, cv, av);
}

auto Xycloid::offset(int param) const -> float {
  auto is_uni = params[param].value > 0.5f;
  return is_uni ? 1.f : 0.f;
}

auto Xycloid::throb_speed() const -> float {
  constexpr auto speed_taper_curvature = 0.8f;
  auto rotation = modulated(THROB_SPEED_KNOB, THROB_SPEED_CV, THROB_SPEED_AV);
  auto scaled = throb_speed_knob_range.scale(rotation);
  auto tapered = Sigmoid::inverse(scaled, speed_taper_curvature);
  return -10.f * tapered * rack::engineGetSampleTime();
}

auto Xycloid::wobble_depth() const -> float {
  auto rotation =
      modulated(WOBBLE_DEPTH_KNOB, WOBBLE_DEPTH_CV, WOBBLE_DEPTH_AV);
  return wobble_depth_range.clamp(rotation);
}

auto Xycloid::wobble_phase_in() const -> float {
  auto rotation = params[WOBBLE_PHASE_KNOB].value;
  return rotation - 0.5f;
}

auto Xycloid::wobble_ratio_range() const -> const Range & {
  static constexpr auto wobble_ratio_max = 16.f;
  static constexpr auto inward_wobble_ratio_range =
      Range{0.f, wobble_ratio_max};
  static constexpr auto outward_wobble_ratio_range =
      Range{0.f, -wobble_ratio_max};
  static constexpr auto bidirectional_wobble_ratio_range =
      Range{wobble_ratio_max, -wobble_ratio_max};
  static constexpr std::array<Range, 3> wobble_ratio_ranges{
      inward_wobble_ratio_range, bidirectional_wobble_ratio_range,
      outward_wobble_ratio_range};

  const auto param = params[WOBBLE_RANGE_SWITCH].value;
  const auto selection = static_cast<int>(param);

  return wobble_ratio_ranges[selection];
}

auto Xycloid::wobble_ratio() const -> float {
  auto wobble_ratio_amount =
      modulated(WOBBLE_RATIO_KNOB, WOBBLE_RATIO_CV, WOBBLE_RATIO_AV);
  auto wobble_ratio =
      wobble_ratio_range().scale(wobble_ratio_amount) + wobble_ratio_offset;
  return is_wobble_ratio_free() ? wobble_ratio : std::round(wobble_ratio);
}

auto Xycloid::x_offset() const -> float { return offset(X_RANGE_SWITCH); }

auto Xycloid::x_gain_in() const -> float {
  return Rotation::gain_multiplier(modulated(X_GAIN_KNOB, X_GAIN_CV));
}

auto Xycloid::y_gain_in() const -> float {
  return Rotation::gain_multiplier(modulated(Y_GAIN_KNOB, Y_GAIN_CV));
}

auto Xycloid::y_offset() const -> float { return offset(Y_RANGE_SWITCH); }

json_t *Xycloid::toJson() {
  json_t *configuration = json_object();
  json_object_set_new(configuration, "musical_wobble_ratios",
                      json_boolean(is_musical_wobble_ratios()));
  return configuration;
}

void Xycloid::fromJson(json_t *configuration) {
  json_t *musical_wobble_ratios =
      json_object_get(configuration, "musical_wobble_ratios");
  set_musical_wobble_ratios(json_is_true(musical_wobble_ratios));
}

} // namespace DHE
