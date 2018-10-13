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
        limit_value(MAX_KNOB, MAX_CV, MAX_ATTENUVERTER_KNOB, MAX_RANGE_SWITCH);
    auto min =
        limit_value(MIN_KNOB, MIN_CV, MIN_ATTENUVERTER_KNOB, MIN_RANGE_SWITCH);
    auto range = Range{min, max};

    auto out =
        ranged_value(LEVEL_KNOB, LEVEL_CV, LEVEL_ATTENUVERTER_KNOB, range);

    outputs[OUT].value = out;
  }

  enum ParameterIds {
    MIN_KNOB,
    LEVEL_KNOB,
    MAX_KNOB,
    MIN_ATTENUVERTER_KNOB,
    LEVEL_ATTENUVERTER_KNOB,
    MAX_ATTENUVERTER_KNOB,
    MIN_RANGE_SWITCH,
    MAX_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { MAX_CV, LEVEL_CV, MIN_CV, INPUT_COUNT };
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

    auto y = 22.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_input<RangerPort>(Ranger::MAX_CV, {left_x, y});
    install_knob<RangerKnobTiny>(Ranger::MAX_ATTENUVERTER_KNOB, {right_x, y});
    y += delta_y;
    install_knob<RangerKnobMedium>(Ranger::MAX_KNOB, {left_x, y});
    install_switch<RangerSwitch2>(Ranger::MAX_RANGE_SWITCH, {right_x, y}, 1, 0);

    y += delta_y + panel_buffer;

    install_input<RangerPort>(Ranger::LEVEL_CV, {left_x, y});
    install_knob<RangerKnobTiny>(Ranger::LEVEL_ATTENUVERTER_KNOB, {right_x, y});
    y += delta_y;
    install_knob<RangerKnobMedium>(Ranger::LEVEL_KNOB, {left_x, y});
    install_output<RangerPort>(Ranger::OUT, {right_x, y});

    y += delta_y + panel_buffer;

    install_input<RangerPort>(Ranger::MIN_CV, {left_x, y});
    install_knob<RangerKnobTiny>(Ranger::MIN_ATTENUVERTER_KNOB, {right_x, y});
    y += delta_y;
    install_knob<RangerKnobMedium>(Ranger::MIN_KNOB, {left_x, y});
    install_switch<RangerSwitch2>(Ranger::MIN_RANGE_SWITCH, {right_x, y}, 1, 0);
  }
};
} // namespace DHE

rack::Model *modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>(
    "DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG);
