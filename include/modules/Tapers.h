#pragma once

#include "Module.h"

namespace DHE {

class Tapers : public Module {
public:
  Tapers();

  void step() override;

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
  auto curvature(int knob, int cv, int av) const -> float;
  auto curvature1() const -> float;
  auto curvature2() const -> float;
  auto is_uni_1() const -> bool;
  auto is_uni_2() const -> bool;
  auto is_s_1() const -> bool;
  auto is_s_2() const -> bool;
  auto level1() const -> float;
  auto level2() const -> float;
  auto taper(float level, bool is_uni, float curve, bool is_s) const -> float;
};

} // namespace DHE
