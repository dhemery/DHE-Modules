#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/range.hpp"
#include "util/sigmoid.hpp"

namespace DHE {

struct TapersLevel {
  rack::Param const &knob;
  rack::Input const &cv;
  rack::Param const &av;

  TapersLevel(rack::Param const &knob, rack::Input const &cv,
              rack::Param const &av)
      : knob{knob}, cv{cv}, av{av} {}

  auto operator()() const -> float { return Module::modulated(knob, cv, av); }
};

struct TapersCurve {
  rack::Param const &knob;
  rack::Input const &cv;
  rack::Param const &av;
  rack::Param const &shape_selector;

  TapersCurve(rack::Param const &knob, rack::Input const &cv,
              rack::Param const &av, rack::Param const &shape_selector)
      : knob{knob}, cv{cv}, av{av}, shape_selector{shape_selector} {}

  auto operator()(float input) const -> float {
    auto const is_s{shape_selector.value > 0.1};
    auto const rotation{Module::modulated(knob, cv, av)};
    auto const curvature{Sigmoid::curvature(rotation)};
    return is_s ? Sigmoid::s_taper(input, curvature)
                : Sigmoid::j_taper(input, curvature);
  }
};

struct TapersPanel {
  TapersLevel const &level;
  TapersCurve const &curve;
  rack::Param const &range_selector;
  rack::Output &output;

  TapersPanel(TapersLevel const &level, TapersCurve const &curve,
              rack::Param const &range_selector, rack::Output &output)
      : level{level}, curve{curve},
        range_selector{range_selector}, output{output} {}

  void step() {
    auto const is_uni{range_selector.value > 0.1};
    auto const &range{is_uni ? Signal::unipolar_range : Signal::bipolar_range};
    output.value = range.scale(curve(level()));
  }
};

struct Tapers : Module {
  TapersLevel const level1{params[LEVEL_1], inputs[LEVEL_1_CV],
                           params[LEVEL_1_AV]};
  TapersCurve const curve1{params[CURVE_1], inputs[CURVE_1_CV],
                           params[CURVE_1_AV], params[SHAPE_1]};
  TapersPanel panel1{level1, curve1, params[RANGE_1], outputs[OUT_1]};

  TapersLevel const level2{params[LEVEL_2], inputs[LEVEL_2_CV],
                           params[LEVEL_2_AV]};
  TapersCurve const curve2{params[CURVE_2], inputs[CURVE_2_CV],
                           params[CURVE_2_AV], params[SHAPE_2]};
  TapersPanel panel2{level2, curve2, params[RANGE_2], outputs[OUT_2]};

  Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    panel1.step();
    panel2.step();
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
