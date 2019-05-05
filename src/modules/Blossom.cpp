#include "modules/Blossom.h"
#include "util/gain.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

Blossom::Blossom() {
    config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

    configParam(SPIN_KNOB, 0.f, 1.f, 0.65f, "Spin", "", 0.f, 2.f, -1.f);
    configCvGain(SPIN_AV, "Spin");

    configKnob(BOUNCE_KNOB, "Bounce");
    configCvGain(BOUNCE_AV, "Bounce");

    configParam(DEPTH_KNOB, 0.f, 1.f, 0.5f, "Depth", "%", 0.f, 100.f);
    configCvGain(DEPTH_AV, "Depth");

    configParam(PHASE_KNOB, 0.f, 1.f, 0.5f, "Phase", "°", 0.f, 360.f, -180.f);
    configCvGain(PHASE_AV, "Phase");

    configGain(X_GAIN_KNOB, "X output");
    configGain(Y_GAIN_KNOB, "Y output");

    configSignalRange(X_RANGE_SWITCH, "X");
    configSignalRange(Y_RANGE_SWITCH, "Y");

    configParam(BOUNCE_LOCK_SWITCH, 0.0, 1.0, 1.0, "Bounce lock");
}

void Blossom::process(const ProcessArgs &args) {
  auto spin_rate = spin(args.sampleTime);
  auto bounce_ratio = is_bounce_free() ? bounce() : std::round(bounce());
  auto bounce_depth = depth();

  spinner.advance(spin_rate, 0.f);
  bouncer.advance(spin_rate * bounce_ratio, phase());

  auto angle = spinner.angle();
  auto radius = (1.f - bounce_depth) + bounce_depth * bouncer.radius();

  auto x = radius * std::cos(angle);
  auto y = radius * std::sin(angle);

  outputs[X_OUT].value = 5.f * x_gain_in() * (x + x_offset());
  outputs[Y_OUT].value = 5.f * y_gain_in() * (y + y_offset());
}

auto Blossom::offset(int param) const -> float {
  auto is_uni = params[param].value > 0.5f;
  return is_uni ? 1.f : 0.f;
}

auto Blossom::bounce() const -> float {
  static constexpr auto bounce_range = Range{1.f, 17.f};
  auto rotation = modulated(BOUNCE_KNOB, BOUNCE_CV, BOUNCE_AV);
  return bounce_range.scale(rotation);
}

auto Blossom::spin(float sample_time) const -> float {
  static constexpr auto spin_range = Range{-1.f, 1.f};
  auto rotation = modulated(SPIN_KNOB, SPIN_CV, SPIN_AV);
  auto scaled = spin_range.scale(rotation);
  auto tapered = Sigmoid::inverse(scaled, speed_curvature);
  return -10.f * tapered * sample_time;
}

auto Blossom::depth() const -> float {
  static constexpr auto depth_range = Range{0.f, 1.f};
  auto rotation = modulated(DEPTH_KNOB, DEPTH_CV, DEPTH_AV);
  return depth_range.clamp(rotation);
}

auto Blossom::is_bounce_free() const -> bool {
  return params[BOUNCE_LOCK_SWITCH].value > 0.1f;
}

auto Blossom::phase() const -> float {
  static constexpr auto phase_range = Range{0.f, 1.f};
  auto rotation = modulated(PHASE_KNOB, PHASE_CV, PHASE_AV);
  return phase_range.clamp(rotation);
}

auto Blossom::x_offset() const -> float { return offset(X_RANGE_SWITCH); }

auto Blossom::x_gain_in() const -> float {
  return Gain::multiplier(modulated(X_GAIN_KNOB, X_GAIN_CV));
}

auto Blossom::y_gain_in() const -> float {
  return Gain::multiplier(modulated(Y_GAIN_KNOB, Y_GAIN_CV));
}

auto Blossom::y_offset() const -> float { return offset(Y_RANGE_SWITCH); }

} // namespace DHE
