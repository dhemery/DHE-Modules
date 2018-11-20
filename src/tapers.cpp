#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/range.hpp"
#include "util/taper.hpp"

namespace DHE {

struct Tapers : Module {
  std::function<float()> const level1{knob(LEVEL_1, LEVEL_1_CV, LEVEL_1_AV)};
  std::function<float()> const curvature1{knob(CURVE_1, TAPER_1_CV, TAPER_1_AV)};
  std::function<bool()> const is_s1{bool_param(SHAPE_1)};
  std::function<const Range &()> const range1{signal_range(int_param(RANGE_1))};
  std::function<float()> const level2{knob(LEVEL_2, LEVEL_2_CV, LEVEL_2_AV)};
  std::function<bool()> const is_s2{bool_param(SHAPE_2)};
  std::function<float()> const curvature2{knob(TAPER_2, TAPER_2_CV, TAPER_2_AV)};
  std::function<const Range &()> const range2{signal_range(int_param(RANGE_2))};

  Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto tapered1 = taper(level1(), curvature1(), is_s1());
    outputs[OUT_1].value = range1().scale(tapered1);

    auto tapered2 = taper(level2(), curvature2(), is_s2());
    outputs[OUT_2].value = range2().scale(tapered2);
  }

  auto taper(float level, float curvature, bool is_s) const -> float {
    return is_s ? Taper::s(level, curvature) : Taper::j(level, curvature);
  }

  enum ParameterIds {
    LEVEL_1,
    LEVEL_1_AV,
    RANGE_1,
    CURVE_1,
    TAPER_1_AV,
    SHAPE_1,
    LEVEL_2,
    LEVEL_2_AV,
    RANGE_2,
    TAPER_2,
    TAPER_2_AV,
    SHAPE_2,
    PARAMETER_COUNT
  };
  enum InputIds {
    LEVEL_1_CV,
    TAPER_1_CV,
    LEVEL_2_CV,
    TAPER_2_CV,
    INPUT_COUNT
  };
  enum OutputIds { OUT_1, OUT_2, OUTPUT_COUNT };
};

struct TapersWidget : public ModuleWidget {
  explicit TapersWidget(rack::Module *module) : ModuleWidget(module, 9, "tapers") {
    auto widget_right_edge = width();

    auto left_x = width()/5.f + 0.333333333f;
    auto center_x = widget_right_edge/2.f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_input(Tapers::LEVEL_1_CV, {left_x, y});
    install_knob("tiny", Tapers::LEVEL_1_AV, {center_x, y});
    install_knob("medium", Tapers::LEVEL_1, {right_x, y});
    y += delta_y;
    install_input(Tapers::TAPER_1_CV, {left_x, y});
    install_knob("tiny", Tapers::TAPER_1_AV, {center_x, y});
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
    install_input(Tapers::TAPER_2_CV, {left_x, y});
    install_knob("tiny", Tapers::TAPER_2_AV, {center_x, y});
    install_knob("medium", Tapers::TAPER_2, {right_x, y});
    y += delta_y;
    install_switch(Tapers::SHAPE_2, {left_x, y});
    install_switch(Tapers::RANGE_2, {center_x, y});
    install_output(Tapers::OUT_2, {right_x, y});
  }
};
} // namespace DHE
rack::Model *modelTapers = rack::Model::create<DHE::Tapers, DHE::TapersWidget>(
    "DHE-Modules", "Tapers", "Tapers", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
