#include <algorithm>

#include <asset.hpp>
#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Tapers : Module {
  Tapers() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  bool is_s_taper(int shape_switch) const {
    return param(shape_switch) > 0.5;
  }

  void step() override {
      auto phase1 = modulated(LEVEL_1_KNOB, LEVEL_1_CV_IN, LEVEL_1_AV_KNOB);
      auto tapered1 = taper(phase1, TAPER_1_KNOB, TAPER_1_CV_IN, TAPER_1_AV_KNOB, SHAPE_1_SWITCH);
      auto range1 = Level::range(param(RANGE_1_SWITCH));
      outputs[OUT_1].value = range1.scale(tapered1);

      auto phase2 = modulated(LEVEL_2_KNOB, LEVEL_2_CV_IN, LEVEL_2_AV_KNOB);
      auto tapered2 = taper(phase2, TAPER_2_KNOB, TAPER_2_CV_IN, TAPER_2_AV_KNOB, SHAPE_2_SWITCH);
      auto range2 = Level::range(param(RANGE_2_SWITCH));
      outputs[OUT_2].value = range2.scale(tapered2);
  }

  float taper(float phase, int knob, int cv, int av, int shape) const {
    auto curvature = modulated(knob, cv, av);
    return is_s_taper(shape) ? Taper::s(phase, curvature) : Taper::j(phase, curvature);
  }

  enum ParameterIds {
    LEVEL_1_KNOB, LEVEL_1_AV_KNOB, RANGE_1_SWITCH,
    TAPER_1_KNOB, TAPER_1_AV_KNOB, SHAPE_1_SWITCH,
    LEVEL_2_KNOB, LEVEL_2_AV_KNOB, RANGE_2_SWITCH,
    TAPER_2_KNOB, TAPER_2_AV_KNOB, SHAPE_2_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
      LEVEL_1_CV_IN, TAPER_1_CV_IN,
      LEVEL_2_CV_IN, TAPER_2_CV_IN,
      INPUT_COUNT };
  enum OutputIds { OUT_1, OUT_2, OUTPUT_COUNT };
};

struct TapersSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  TapersSwitch2() {
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/tapers/switch-2-low.svg")));
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/tapers/switch-2-high.svg")));
  }
};

struct TapersWidget : public ModuleWidget {
  TapersWidget(rack::Module *module)
      : ModuleWidget(module, 9, "tapers") {
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
    install_switch<TapersSwitch2>(Tapers::SHAPE_1_SWITCH, {left_x, y}, 1, 0);
    install_switch<TapersSwitch2>(Tapers::RANGE_1_SWITCH, {center_x, y}, 1, 0);
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
    install_switch<TapersSwitch2>(Tapers::SHAPE_2_SWITCH, {left_x, y}, 1, 0);
    install_switch<TapersSwitch2>(Tapers::RANGE_2_SWITCH, {center_x, y}, 1, 0);
    install_output(Tapers::OUT_2, {right_x, y});

  }
};
} // namespace DHE
rack::Model *modelTapers = rack::Model::create<DHE::Tapers, DHE::TapersWidget>(
    "DHE-Modules", "Tapers", "Tapers", rack::UTILITY_TAG, rack::WAVESHAPER_TAG);
