#include "modules/Tapers.h"

#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

auto taper(LevelControl *level, float curve, bool is_s) -> float {
  auto tapered = Sigmoid::taper(level->rotation(), curve, is_s);
  return level->range()->scale(tapered);
}

Tapers::Tapers() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(LEVEL_1_KNOB, 0.f, 1.f, 0.5f, "Taper 1 level", "%", 0.f, 100.f,
              0.f);
  configSignalRange(RANGE_1_SWITCH, "Taper 1 level", false);
  configCvGain(LEVEL_1_AV, "Taper 1 level");
  configParam(CURVE_1_KNOB, 0.f, 1.f, 0.5f, "Taper 1 curvature", "%", 0.f,
              100.f, 0.f);
  configCvGain(CURVE_1_AV, "Taper 1 level");
  configParam(SHAPE_1_SWITCH, 0.f, 1.f, 0.f, "Taper 1 shape");

  configParam(LEVEL_2_KNOB, 0.f, 1.f, 0.5f, "Taper 2 level", "%", 0.f, 100.f,
              0.f);
  configSignalRange(RANGE_2_SWITCH, "Taper 2 level", false);
  configCvGain(LEVEL_2_AV, "Taper 2 level");
  configParam(CURVE_2_KNOB, 0.f, 1.f, 0.5f, "Taper 2 curvature", "%", 0.f,
              100.f, 0.f);
  configCvGain(CURVE_2_AV, "Taper 2 level");
  configParam(SHAPE_2_SWITCH, 0.f, 1.f, 0.f, "Taper 2 shape");

  level1 = std::unique_ptr<LevelControl>(
      new LevelControl(params[LEVEL_1_KNOB], params[RANGE_1_SWITCH],
                       inputs[LEVEL_1_CV], params[LEVEL_1_AV]));
  level2 = std::unique_ptr<LevelControl>(
      new LevelControl(params[LEVEL_2_KNOB], params[RANGE_2_SWITCH],
                       inputs[LEVEL_2_CV], params[LEVEL_2_AV]));
}

void Tapers::process(const ProcessArgs &args) {
  outputs[OUT_1].setVoltage(taper(level1.get(), curvature1(), is_s_1()));
  outputs[OUT_2].setVoltage(taper(level2.get(), curvature2(), is_s_2()));
}

auto Tapers::curvature(int knob, int cv, int av) -> float {
  auto curvature = modulated(knob, cv, av);
  return Sigmoid::curvature(curvature);
}

auto Tapers::curvature1() -> float {
  return curvature(CURVE_1_KNOB, CURVE_1_CV, CURVE_1_AV);
}

auto Tapers::curvature2() -> float {
  return curvature(CURVE_2_KNOB, CURVE_2_CV, CURVE_2_AV);
}

auto Tapers::is_s_1() -> bool {
  return params[SHAPE_1_SWITCH].getValue() > 0.5f;
}

auto Tapers::is_s_2() -> bool {
  return params[SHAPE_2_SWITCH].getValue() > 0.5f;
}

} // namespace DHE
