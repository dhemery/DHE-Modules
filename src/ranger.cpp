#include <algorithm>

#include <asset.hpp>
#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module.hpp"
#include "module-widget.hpp"

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Ranger : Module {
  Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
  }


  enum ParameterIds {
    MAX_ATTENUVERTER_KNOB,
    LEVEL_ATTENUVERTER_KNOB,
    MIN_ATTENUVERTER_KNOB,
    MAX_KNOB,
    LEVEL_KNOB,
    MIN_KNOB,
    MAX_CLAMP_BUTTON,
    LEVEL_CLAMP_BUTTON,
    MIN_CLAMP_BUTTON,
    MAX_RANGE_SWITCH,
    MIN_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    MAX_CV,
    LEVEL_CV,
    MIN_CV,
    INPUT_COUNT
  };
  enum OutputIds {
    OUT,
    OUTPUT_COUNT
  };
};

struct RangerButtonNormal : rack::SVGSwitch, rack::ToggleSwitch {
  RangerButtonNormal() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/button-normal-off.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/button-normal-on.svg")));
  }
};

struct RangerKnobLarge : rack::RoundKnob {
  RangerKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct RangerKnobTiny : rack::RoundKnob {
  RangerKnobTiny() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct RangerPort : rack::SVGPort {
  RangerPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/ranger/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct RangerSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  RangerSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/ranger/switch-2-high.svg")));
  }
};

struct RangerWidget : public ModuleWidget {
  RangerWidget(rack::Module *module) : ModuleWidget(module, 8, "res/ranger/panel.svg") {
    auto widget_right_edge = width();

    auto left_x = width()/6.f + 0.333333333f;
    auto center_x = widget_right_edge/2.f;
    auto right_x = widget_right_edge - left_x;

    auto y = 24.f;
    auto delta_y = 16.f;
    auto panel_buffer = 4.f;

    install_knob<RangerKnobTiny>(Ranger::MAX_ATTENUVERTER_KNOB, {left_x, y});
    install_switch<RangerButtonNormal>(Ranger::MAX_CLAMP_BUTTON, {right_x, y});
    install_knob<RangerKnobLarge>(Ranger::MAX_KNOB, {center_x, y + delta_y/2.f});
    y += delta_y;
    install_input<RangerPort>(Ranger::MAX_CV, {left_x, y});
    install_switch<RangerSwitch2>(Ranger::MAX_RANGE_SWITCH, {right_x, y}, 1, 1);

    y += delta_y + panel_buffer;

    install_knob<RangerKnobTiny>(Ranger::LEVEL_ATTENUVERTER_KNOB, {left_x, y});
    install_switch<RangerButtonNormal>(Ranger::LEVEL_CLAMP_BUTTON, {right_x, y});
    install_knob<RangerKnobLarge>(Ranger::LEVEL_KNOB, {center_x, y + delta_y/2.f});
    y += delta_y;
    install_input<RangerPort>(Ranger::LEVEL_CV, {left_x, y});
    install_output<RangerPort>(Ranger::OUT, {right_x, y});

    y += delta_y + panel_buffer;

    install_knob<RangerKnobTiny>(Ranger::MIN_ATTENUVERTER_KNOB, {left_x, y});
    install_switch<RangerButtonNormal>(Ranger::MIN_CLAMP_BUTTON, {right_x, y});
    install_knob<RangerKnobLarge>(Ranger::MIN_KNOB, {center_x, y + delta_y/2.f});
    y += delta_y;
    install_input<RangerPort>(Ranger::MIN_CV, {left_x, y});
    install_switch<RangerSwitch2>(Ranger::MIN_RANGE_SWITCH, {right_x, y}, 1, 1);
  }
};
}
rack::Model *modelRanger = rack::Model::create<DHE::Ranger, DHE::RangerWidget>("DHE-Modules", "Ranger", "Ranger", rack::UTILITY_TAG);
