#include "modules/Blossom.h"

#include "util/gain.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace dhe {

Blossom::Blossom() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(SPIN_KNOB, 0.f, 1.f, 0.65f, "Spin", "", 0.f, 2.f, -1.f);
  configCvGain(SPIN_AV, "Spin");

  configKnob(BOUNCE_KNOB, "Bounce");
  configCvGain(BOUNCE_AV, "Bounce");

  configParam(DEPTH_KNOB, 0.f, 1.f, 0.5f, "Depth", "%", 0.f, 100.f);
  configCvGain(DEPTH_AV, "Depth");

  configParam(PHASE_KNOB, 0.f, 1.f, 0.5f, "Bounce phase offset", "°", 0.f,
              360.f, -180.f);
  configCvGain(PHASE_AV, "Bounce phase offset");

  configGain(X_GAIN_KNOB, "X output");
  configGain(Y_GAIN_KNOB, "Y output");

  configSignalRange(X_RANGE_SWITCH, "X", false);
  configSignalRange(Y_RANGE_SWITCH, "Y", false);

  configParam(BOUNCE_LOCK_SWITCH, 0.0, 1.0, 1.0, "Bounce lock");
}

void Blossom::process(const ProcessArgs &args) {
  auto spinRate = spin(args.sampleTime);
  auto bounceRatio = is_bounce_free() ? bounce() : std::round(bounce());
  auto bounceDepth = depth();

  spinner.advance(spinRate, 0.f);
  bouncer.advance(spinRate * bounceRatio, phase());

  auto angle = spinner.angle();
  auto radius = (1.f - bounceDepth) + bounceDepth * bouncer.radius();

  auto x = radius * std::cos(angle);
  auto y = radius * std::sin(angle);

  outputs[X_OUT].setVoltage(5.f * x_gain_in() * (x + x_offset()));
  outputs[Y_OUT].setVoltage(5.f * y_gain_in() * (y + y_offset()));
}

auto Blossom::offset(int param) -> float {
  auto is_uni = params[param].getValue() > 0.5f;
  return is_uni ? 1.f : 0.f;
}

auto Blossom::bounce() -> float {
  static constexpr auto bounceRange = Range{1.f, 17.f};
  auto rotation = modulated(BOUNCE_KNOB, BOUNCE_CV, BOUNCE_AV);
  return bounceRange.scale(rotation);
}

auto Blossom::spin(float sample_time) -> float {
  static constexpr auto spinRange = Range{-1.f, 1.f};
  auto rotation = modulated(SPIN_KNOB, SPIN_CV, SPIN_AV);
  auto scaled = spinRange.scale(rotation);
  auto tapered = sigmoid::inverse(scaled, speed_curvature);
  return -10.f * tapered * sample_time;
}

auto Blossom::depth() -> float {
  static constexpr auto depthRange = Range{0.f, 1.f};
  auto rotation = modulated(DEPTH_KNOB, DEPTH_CV, DEPTH_AV);
  return depthRange.clamp(rotation);
}

auto Blossom::is_bounce_free() -> bool {
  return params[BOUNCE_LOCK_SWITCH].getValue() > 0.1f;
}

auto Blossom::phase() -> float {
  static constexpr auto phaseRange = Range{0.f, 1.f};
  auto rotation = modulated(PHASE_KNOB, PHASE_CV, PHASE_AV);
  return phaseRange.clamp(rotation);
}

auto Blossom::x_offset() -> float { return offset(X_RANGE_SWITCH); }

auto Blossom::x_gain_in() -> float {
  return Gain::multiplier(modulated(X_GAIN_KNOB, X_GAIN_CV));
}

auto Blossom::y_gain_in() -> float {
  return Gain::multiplier(modulated(Y_GAIN_KNOB, Y_GAIN_CV));
}

auto Blossom::y_offset() -> float { return offset(Y_RANGE_SWITCH); }

} // namespace dhe
