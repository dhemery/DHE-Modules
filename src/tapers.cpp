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

  void step_knob(int index) {
    auto range = Level::range(param(RANGE_SWITCH_1 + index));
    auto rotation = modulated(LEVEL_KNOB_1 + index, CV_1 + index,
                              ATTENUVERTER_KNOB_1 + index);
    outputs[OUT_1 + index].value = range.scale(rotation);
  }

  void step() override {
    step_knob(0);
    step_knob(1);
  }

  enum ParameterIds {
    LEVEL_KNOB_1,
    LEVEL_KNOB_2,
    ATTENUVERTER_KNOB_1,
    ATTENUVERTER_KNOB_2,
    SHAPE_SWITCH_1,
    SHAPE_SWITCH_2,
    CURVE_KNOB_1,
    CURVE_KNOB_2,
    RANGE_SWITCH_1,
    RANGE_SWITCH_2,
    PARAMETER_COUNT
  };
  enum InputIds { CV_1, CV_2, CV_3, INPUT_COUNT };
  enum OutputIds { OUT_1, OUT_2, OUT_3, OUTPUT_COUNT };
};

struct TapersButtonNormal : rack::SVGSwitch, rack::ToggleSwitch {
  TapersButtonNormal() {
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/tapers/button-normal-off.svg")));
    addFrame(rack::SVG::load(
        rack::assetPlugin(plugin, "res/tapers/button-normal-on.svg")));
  }
};

struct TapersKnobLarge : rack::RoundKnob {
  TapersKnobLarge() {
    setSVG(
        rack::SVG::load(rack::assetPlugin(plugin, "res/tapers/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct TapersKnobTiny : rack::RoundKnob {
  TapersKnobTiny() {
    setSVG(
        rack::SVG::load(rack::assetPlugin(plugin, "res/tapers/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct TapersPort : rack::SVGPort {
  TapersPort() {
    background->svg =
        rack::SVG::load(assetPlugin(plugin, "res/tapers/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
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
      : ModuleWidget(module, 8, "res/tapers/panel.svg") {
    auto widget_right_edge = width();

    auto left_x = width() / 6.f + 0.333333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob<TapersKnobTiny>(Tapers::ATTENUVERTER_KNOB_1, {left_x, y});
    install_knob<TapersKnobLarge>(Tapers::LEVEL_KNOB_1, {center_x, y});
    y += delta_y;
    install_input<TapersPort>(Tapers::CV_1, {left_x, y});
    install_output<TapersPort>(Tapers::OUT_1, {center_x, y});
    install_switch<TapersSwitch2>(Tapers::RANGE_SWITCH_1, {right_x, y}, 1, 1);

    y += delta_y + panel_buffer;

    install_knob<TapersKnobTiny>(Tapers::ATTENUVERTER_KNOB_2, {left_x, y});
    install_knob<TapersKnobLarge>(Tapers::LEVEL_KNOB_2, {center_x, y});
    y += delta_y;
    install_input<TapersPort>(Tapers::CV_2, {left_x, y});
    install_output<TapersPort>(Tapers::OUT_2, {center_x, y});
    install_switch<TapersSwitch2>(Tapers::RANGE_SWITCH_2, {right_x, y}, 1, 1);
  }
};
} // namespace DHE
rack::Model *modelTapers = rack::Model::create<DHE::Tapers, DHE::TapersWidget>(
    "DHE-Modules", "Tapers", "Tapers", rack::UTILITY_TAG);
