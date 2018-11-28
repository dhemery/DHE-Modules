#include <utility>

#include "dhe-modules.h"
#include "module-widget.h"

#include "controls/knob.h"
#include "controls/signal.h"
#include "util/sigmoid.h"

namespace DHE {

struct Tapers : rack::Module {
  const Knob curvature1{this, CURVE_1, CURVE_1_CV, CURVE_1_AV};
  const Button is_s1{this, SHAPE_1};
  const Switch2<const Range &> range1 = Signal::range_switch(this, RANGE_1);
  const Sigmoid::Shaper shaper1{curvature1, is_s1};
  const Knob level1{this, LEVEL_1, LEVEL_1_CV, LEVEL_1_AV};

  const Knob curvature2{this, CURVE_2, CURVE_2_CV, CURVE_2_AV};
  const Button is_s2{this, SHAPE_2};
  const Switch2<const Range &> range2 = Signal::range_switch(this, RANGE_2);
  const Sigmoid::Shaper shaper2{curvature2, is_s2};
  const Knob level2{this, LEVEL_2, LEVEL_2_CV, LEVEL_2_AV};

  Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    outputs[OUT_1].value = range1().scale(shaper1(level1()));
    outputs[OUT_2].value = range2().scale(shaper2(level2()));
  }

  enum ParameterIds {
    LEVEL_1,
    LEVEL_1_AV,
    RANGE_1,
    CURVE_1,
    CURVE_1_AV,
    SHAPE_1,
    LEVEL_2,
    LEVEL_2_AV,
    RANGE_2,
    CURVE_2,
    CURVE_2_AV,
    SHAPE_2,
    PARAMETER_COUNT
  };
  enum InputIds { LEVEL_1_CV, CURVE_1_CV, LEVEL_2_CV, CURVE_2_CV, INPUT_COUNT };
  enum OutputIds { OUT_1, OUT_2, OUTPUT_COUNT };
};

struct TapersWidget : public ModuleWidget {
  explicit TapersWidget(rack::Module *module)
      : ModuleWidget(module, 9, "tapers") {
    auto widget_right_edge = width();

    auto left_x = width() / 5.f + 0.333333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_input(Tapers::LEVEL_1_CV, {left_x, y});
    install_knob("tiny", Tapers::LEVEL_1_AV, {center_x, y});
    install_knob("medium", Tapers::LEVEL_1, {right_x, y});
    y += delta_y;
    install_input(Tapers::CURVE_1_CV, {left_x, y});
    install_knob("tiny", Tapers::CURVE_1_AV, {center_x, y});
    install_knob("medium", Tapers::CURVE_1, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_1, {left_x, y});
    install_switch(Tapers::RANGE_1, {center_x, y});
    install_output(Tapers::OUT_1, {right_x, y});

    y += delta_y + panel_buffer;

    install_input(Tapers::LEVEL_2_CV, {left_x, y});
    install_knob("tiny", Tapers::LEVEL_2_AV, {center_x, y});
    install_knob("medium", Tapers::LEVEL_2, {right_x, y});
    y += delta_y;
    install_input(Tapers::CURVE_2_CV, {left_x, y});
    install_knob("tiny", Tapers::CURVE_2_AV, {center_x, y});
    install_knob("medium", Tapers::CURVE_2, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_2, {left_x, y});
    install_switch(Tapers::RANGE_2, {center_x, y});
    install_output(Tapers::OUT_2, {right_x, y});
  }
};
} // namespace DHE
rack::Model *modelTapers = rack::Model::create<DHE::Tapers, DHE::TapersWidget>(
    "DHE-Modules", "Tapers", "Tapers", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
