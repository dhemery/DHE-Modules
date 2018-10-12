#include <algorithm>

#include <asset.hpp>
#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Knobs : Module {
  Knobs() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step_knob(int index) {
    auto range = Level::range(param(RANGE_SWITCH_1 + index));
    auto rotation = modulated(LEVEL_KNOB_1 + index, CV_1 + index,
                              ATTENUVERTER_KNOB_1 + index);
    auto scaled = range.scale(rotation);
    auto is_clamped = param(CLAMP_BUTTON_1 + index) > 0.5f;
    auto out = is_clamped ? range.clamp(scaled) : scaled;
    outputs[OUT_1 + index].value = out;
  }

  void step() override {
    step_knob(0);
    step_knob(1);
    step_knob(2);
  }

  enum ParameterIds {
    ATTENUVERTER_KNOB_1,
    ATTENUVERTER_KNOB_2,
    ATTENUVERTER_KNOB_3,
    LEVEL_KNOB_1,
    LEVEL_KNOB_2,
    LEVEL_KNOB_3,
    CLAMP_BUTTON_1,
    CLAMP_BUTTON_2,
    CLAMP_BUTTON_3,
    RANGE_SWITCH_1,
    RANGE_SWITCH_2,
    RANGE_SWITCH_3,
    PARAMETER_COUNT
  };
  enum InputIds { CV_1, CV_2, CV_3, INPUT_COUNT };
  enum OutputIds { OUT_1, OUT_2, OUT_3, OUTPUT_COUNT };
};

struct KnobsButtonNormal : rack::SVGSwitch, rack::ToggleSwitch {
  KnobsButtonNormal() {
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/knobs/button-normal-off.svg")));
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/knobs/button-normal-on.svg")));
  }
};

struct KnobsKnobLarge : rack::RoundKnob {
  KnobsKnobLarge() {
    setSVG(
        rack::SVG::load(rack::assetPlugin(plugin, "res/knobs/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct KnobsKnobTiny : rack::RoundKnob {
  KnobsKnobTiny() {
    setSVG(
        rack::SVG::load(rack::assetPlugin(plugin, "res/knobs/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct KnobsPort : rack::SVGPort {
  KnobsPort() {
    background->svg =
        rack::SVG::load(assetPlugin(plugin, "res/knobs/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct KnobsSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  KnobsSwitch2() {
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/knobs/switch-2-low.svg")));
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/knobs/switch-2-high.svg")));
  }
};

struct KnobsWidget : public ModuleWidget {
  KnobsWidget(rack::Module *module)
      : ModuleWidget(module, 8, "res/knobs/panel.svg") {
    auto widget_right_edge = width();

    auto left_x = width() / 6.f + 0.333333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob<KnobsKnobTiny>(Knobs::ATTENUVERTER_KNOB_1, {left_x, y});
    install_knob<KnobsKnobLarge>(Knobs::LEVEL_KNOB_1, {center_x, y});
    install_switch<KnobsButtonNormal>(Knobs::CLAMP_BUTTON_1, {right_x, y});
    y += delta_y;
    install_input<KnobsPort>(Knobs::CV_1, {left_x, y});
    install_output<KnobsPort>(Knobs::OUT_1, {center_x, y});
    install_switch<KnobsSwitch2>(Knobs::RANGE_SWITCH_1, {right_x, y}, 1, 1);

    y += delta_y + panel_buffer;

    install_knob<KnobsKnobTiny>(Knobs::ATTENUVERTER_KNOB_2, {left_x, y});
    install_knob<KnobsKnobLarge>(Knobs::LEVEL_KNOB_2, {center_x, y});
    install_switch<KnobsButtonNormal>(Knobs::CLAMP_BUTTON_2, {right_x, y});
    y += delta_y;
    install_input<KnobsPort>(Knobs::CV_2, {left_x, y});
    install_output<KnobsPort>(Knobs::OUT_2, {center_x, y});
    install_switch<KnobsSwitch2>(Knobs::RANGE_SWITCH_2, {right_x, y}, 1, 1);

    y += delta_y + panel_buffer;

    install_knob<KnobsKnobTiny>(Knobs::ATTENUVERTER_KNOB_3, {left_x, y});
    install_knob<KnobsKnobLarge>(Knobs::LEVEL_KNOB_3, {center_x, y});
    install_switch<KnobsButtonNormal>(Knobs::CLAMP_BUTTON_3, {right_x, y});
    y += delta_y;
    install_input<KnobsPort>(Knobs::CV_3, {left_x, y});
    install_output<KnobsPort>(Knobs::OUT_3, {center_x, y});
    install_switch<KnobsSwitch2>(Knobs::RANGE_SWITCH_3, {right_x, y}, 1, 1);
  }
};
} // namespace DHE
rack::Model *modelKnobs = rack::Model::create<DHE::Knobs, DHE::KnobsWidget>(
    "DHE-Modules", "Knobs", "Knobs", rack::UTILITY_TAG);
