#pragma once

#include "Module.h"

namespace DHE {

class Tapers : public Module {
public:
  Tapers();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    LEVEL_1_KNOB,
    LEVEL_1_AV,
    RANGE_1_SWITCH,
    CURVE_1_KNOB,
    CURVE_1_AV,
    SHAPE_1_SWITCH,
    LEVEL_2_KNOB,
    LEVEL_2_AV,
    RANGE_2_SWITCH,
    CURVE_2_KNOB,
    CURVE_2_AV,
    SHAPE_2_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { LEVEL_1_CV, CURVE_1_CV, LEVEL_2_CV, CURVE_2_CV, INPUT_COUNT };
  enum OutputIds { OUT_1, OUT_2, OUTPUT_COUNT };

private:
  auto curvature(int knob, int cv, int av) -> float;
  auto curvature1() -> float;
  auto curvature2() -> float;
  auto is_uni_1() -> bool;
  auto is_uni_2() -> bool;
  auto is_s_1() -> bool;
  auto is_s_2() -> bool;
  auto level1() -> float;
  auto level2() -> float;
  auto taper(float level, bool is_uni, float curve, bool is_s) const -> float;
};

} // namespace DHE
