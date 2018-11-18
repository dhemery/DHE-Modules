#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/range.hpp"
#include "util/taper.hpp"

namespace DHE {

struct Tapers : Module {
  const std::function<float()> level1_knob =
      knob(LEVEL_1_KNOB, LEVEL_1_CV_IN, LEVEL_1_AV_KNOB);
  const std::function<float()> taper1_knob =
      knob(TAPER_1_KNOB, TAPER_1_CV_IN, TAPER_1_AV_KNOB);
  const std::function<const Range &()> range1 = range_switch(RANGE_1_SWITCH);
  const std::function<float()> level2_knob =
      knob(LEVEL_2_KNOB, LEVEL_2_CV_IN, LEVEL_2_AV_KNOB);
  const std::function<float()> taper2_knob =
      knob(TAPER_2_KNOB, TAPER_2_CV_IN, TAPER_2_AV_KNOB);
  const std::function<const Range &()> range2 = range_switch(RANGE_2_SWITCH);

  Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  bool is_s_taper(int shape_switch) const { return param(shape_switch) > 0.5; }

  void step() override {
    auto tapered1 = taper(level1_knob(), taper1_knob(), SHAPE_1_SWITCH);
    outputs[OUT_1].value = range1().scale(tapered1);

    auto tapered2 = taper(level2_knob(), taper1_knob(), SHAPE_2_SWITCH);
    outputs[OUT_2].value = range2().scale(tapered2);
  }

  float taper(float level, float curvature, int shape) const {
    return is_s_taper(shape) ? Taper::s(level, curvature)
                             : Taper::j(level, curvature);
  }

  enum ParameterIds {
    LEVEL_1_KNOB,
    LEVEL_1_AV_KNOB,
    RANGE_1_SWITCH,
    TAPER_1_KNOB,
    TAPER_1_AV_KNOB,
    SHAPE_1_SWITCH,
    LEVEL_2_KNOB,
    LEVEL_2_AV_KNOB,
    RANGE_2_SWITCH,
    TAPER_2_KNOB,
    TAPER_2_AV_KNOB,
    SHAPE_2_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    LEVEL_1_CV_IN,
    TAPER_1_CV_IN,
    LEVEL_2_CV_IN,
    TAPER_2_CV_IN,
    INPUT_COUNT
  };
  enum OutputIds { OUT_1, OUT_2, OUTPUT_COUNT };
};

struct TapersWidget : public ModuleWidget {
  explicit TapersWidget(rack::Module *module) : ModuleWidget(module, 9, "tapers") {
    auto widget_right_edge = width();

    auto left_x = width() / 5.f + 0.333333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_input(Tapers::LEVEL_1_CV_IN, {left_x, y});
    install_knob("tiny", Tapers::LEVEL_1_AV_KNOB, {center_x, y});
    install_knob("medium", Tapers::LEVEL_1_KNOB, {right_x, y});
    y += delta_y;
    install_input(Tapers::TAPER_1_CV_IN, {left_x, y});
    install_knob("tiny", Tapers::TAPER_1_AV_KNOB, {center_x, y});
    install_knob("medium", Tapers::TAPER_1_KNOB, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_1_SWITCH, {left_x, y});
    install_switch(Tapers::RANGE_1_SWITCH, {center_x, y});
    install_output(Tapers::OUT_1, {right_x, y});

    y += delta_y + panel_buffer;

    install_input(Tapers::LEVEL_2_CV_IN, {left_x, y});
    install_knob("tiny", Tapers::LEVEL_2_AV_KNOB, {center_x, y});
    install_knob("medium", Tapers::LEVEL_2_KNOB, {right_x, y});
    y += delta_y;
    install_input(Tapers::TAPER_2_CV_IN, {left_x, y});
    install_knob("tiny", Tapers::TAPER_2_AV_KNOB, {center_x, y});
    install_knob("medium", Tapers::TAPER_2_KNOB, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_2_SWITCH, {left_x, y});
    install_switch(Tapers::RANGE_2_SWITCH, {center_x, y});
    install_output(Tapers::OUT_2, {right_x, y});
  }
};
} // namespace DHE
rack::Model *modelTapers = rack::Model::create<DHE::Tapers, DHE::TapersWidget>(
    "DHE-Modules", "Tapers", "Tapers", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
