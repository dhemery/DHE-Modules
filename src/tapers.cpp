#include <utility>

#include "dhe-modules.h"
#include "module-widget.h"

#include "controls/knob.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

struct Tapers : rack::Module {
  Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto level = modulated(this, LEVEL_1_KNOB, LEVEL_1_CV, LEVEL_1_AV);
    auto curvature = Sigmoid::curvature(
        modulated(this, CURVE_1_KNOB, CURVE_1_CV, CURVE_1_AV));
    auto is_s = params[SHAPE_1_SWITCH].value > 0.5f;
    auto tapered = Sigmoid::taper(level, curvature, is_s);
    auto is_uni = params[RANGE_1_SWITCH].value > 0.5f;
    auto scaled = Signal::range(is_uni).scale(tapered);
    outputs[OUT_1].value = scaled;

    level = modulated(this, LEVEL_2_KNOB, LEVEL_2_CV, LEVEL_2_AV);
    curvature = Sigmoid::curvature(
        modulated(this, CURVE_2_KNOB, CURVE_2_CV, CURVE_2_AV));
    is_s = params[SHAPE_2_SWITCH].value > 0.5f;
    tapered = Sigmoid::taper(level, curvature, is_s);
    is_uni = params[RANGE_2_SWITCH].value > 0.5f;
    scaled = Signal::range(is_uni).scale(tapered);
    outputs[OUT_2].value = scaled;
  }

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
    install_knob("medium", Tapers::LEVEL_1_KNOB, {right_x, y});
    y += delta_y;
    install_input(Tapers::CURVE_1_CV, {left_x, y});
    install_knob("tiny", Tapers::CURVE_1_AV, {center_x, y});
    install_knob("medium", Tapers::CURVE_1_KNOB, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_1_SWITCH, {left_x, y});
    install_switch(Tapers::RANGE_1_SWITCH, {center_x, y});
    install_output(Tapers::OUT_1, {right_x, y});

    y += delta_y + panel_buffer;

    install_input(Tapers::LEVEL_2_CV, {left_x, y});
    install_knob("tiny", Tapers::LEVEL_2_AV, {center_x, y});
    install_knob("medium", Tapers::LEVEL_2_KNOB, {right_x, y});
    y += delta_y;
    install_input(Tapers::CURVE_2_CV, {left_x, y});
    install_knob("tiny", Tapers::CURVE_2_AV, {center_x, y});
    install_knob("medium", Tapers::CURVE_2_KNOB, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_2_SWITCH, {left_x, y});
    install_switch(Tapers::RANGE_2_SWITCH, {center_x, y});
    install_output(Tapers::OUT_2, {right_x, y});
  }
};
} // namespace DHE
rack::Model *modelTapers = rack::Model::create<DHE::Tapers, DHE::TapersWidget>(
    "DHE-Modules", "Tapers", "Tapers", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
