#include <algorithm>

#include <asset.hpp>
#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Ranger : Module {
  Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  float ranged_value(int knob_param, int cv_param, int av_param,
                     const Range &range) {
    auto rotation = modulated(knob_param, cv_param, av_param);
    return range.scale(rotation);
  }

  float limit_value(int knob_param, int cv_input, int av_param,
                    int range_param) {
    auto range = Level::range(param(range_param));
    return ranged_value(knob_param, cv_input, av_param, range);
  }

  void step() override {
    auto max =
        limit_value(LIMIT_1_KNOB, LIMIT_1_CV_IN, LIMIT_1_AV_KNOB, LIMIT_1_RANGE_SWITCH);
    auto min =
        limit_value(LIMIT_2_KNOB, LIMIT_2_CV_IN, LIMIT_2_AV_KNOB, LIMIT_2_RANGE_SWITCH);
    auto range = Range{min, max};

    auto out =
        ranged_value(LEVEL_KNOB, LEVEL_CV_IN, LEVEL_AV_KNOB, range);

    outputs[OUT].value = out;
  }

  enum ParameterIds {
      LEVEL_KNOB,
      LEVEL_AV_KNOB,
      LIMIT_1_KNOB,
      LIMIT_1_AV_KNOB,
      LIMIT_1_RANGE_SWITCH,
      LIMIT_2_KNOB,
      LIMIT_2_AV_KNOB,
      LIMIT_2_RANGE_SWITCH,
      PARAMETER_COUNT
  };
  enum InputIds { LEVEL_CV_IN, LIMIT_1_CV_IN, LIMIT_2_CV_IN, INPUT_COUNT };
  enum OutputIds { OUT, OUTPUT_COUNT };
};

struct RangerButtonNormal : rack::SVGSwitch, rack::ToggleSwitch {
  RangerButtonNormal() {
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/ranger/button-normal-off.svg")));
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/ranger/button-normal-on.svg")));
  }
};

struct RangerKnobMedium : rack::RoundKnob {
  RangerKnobMedium() {
    setSVG(rack::SVG::load(
        rack::assetPlugin(plugin, "res/ranger/knob-medium.svg")));
    shadow->opacity = 0.f;
  }
};

struct RangerKnobTiny : rack::RoundKnob {
  RangerKnobTiny() {
    setSVG(
        rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct RangerPort : rack::SVGPort {
  RangerPort() {
    background->svg =
        rack::SVG::load(assetPlugin(plugin, "res/ranger/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct RangerSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  RangerSwitch2() {
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/ranger/switch-2-low.svg")));
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/ranger/switch-2-high.svg")));
  }
};

struct RangerWidget : public ModuleWidget {
  RangerWidget(rack::Module *module)
      : ModuleWidget(module, 6, "res/ranger/panel.svg") {
    auto widget_right_edge = width();

    auto left_x = width() / 3.5f + 0.333333333f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob<RangerKnobMedium>(Ranger::LEVEL_KNOB, {left_x, y});
    install_output<RangerPort>(Ranger::OUT, {right_x, y});
    y += delta_y;
    install_input<RangerPort>(Ranger::LEVEL_CV_IN, {left_x, y});
    install_knob<RangerKnobTiny>(Ranger::LEVEL_AV_KNOB, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob<RangerKnobMedium>(Ranger::LIMIT_1_KNOB, {left_x, y});
    install_switch<RangerSwitch2>(Ranger::LIMIT_1_RANGE_SWITCH, {right_x, y}, 1, 0);
    y += delta_y;
    install_input<RangerPort>(Ranger::LIMIT_1_CV_IN, {left_x, y});
    install_knob<RangerKnobTiny>(Ranger::LIMIT_1_AV_KNOB, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob<RangerKnobMedium>(Ranger::LIMIT_2_KNOB, {left_x, y});
    install_switch<RangerSwitch2>(Ranger::LIMIT_2_RANGE_SWITCH, {right_x, y}, 1, 0);
    y += delta_y;
    install_input<RangerPort>(Ranger::LIMIT_2_CV_IN, {left_x, y});
    install_knob<RangerKnobTiny>(Ranger::LIMIT_2_AV_KNOB, {right_x, y});
  }
};
} // namespace DHE

rack::Model *modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>(
    "DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG);
