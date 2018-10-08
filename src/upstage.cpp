#include <algorithm>

#include <asset.hpp>
#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module.hpp"
#include "module-widget.hpp"

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Upstage : Module {
  Upstage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  float envelope_out() const {
    const auto &range = Level::range(params[LEVEL_SWITCH].value);
    auto rotation = modulated(LEVEL_KNOB, LEVEL_CV);
    return Level::scaled(rotation, range);
  }

  bool is_waiting() const {
    return std::max(inputs[WAIT_IN].value, gate_button(WAIT_BUTTON)) > 0.5f;
  }

  void step() override {
    outputs[TRIG_OUT].value = trigger_out();
    outputs[ENVELOPE_OUT].value = envelope_out();
  }

  float trigger_in() const {
    return param(TRIG_BUTTON) > 0.5f ? 10.f : input(TRIG_IN);
  }

  float trigger_out() const {
    return is_waiting() ? 0.f : trigger_in();
  }

  enum ParameterIds {
    LEVEL_KNOB,
    TRIG_BUTTON,
    WAIT_BUTTON,
    LEVEL_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    TRIG_IN,
    WAIT_IN,
    LEVEL_CV,
    INPUT_COUNT
  };
  enum OutputIds {
    TRIG_OUT,
    ENVELOPE_OUT,
    OUTPUT_COUNT
  };
};

struct UpstageButtonNormal : rack::SVGSwitch, rack::MomentarySwitch {
  UpstageButtonNormal() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/upstage/button-normal-off.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/upstage/button-normal-on.svg")));
  }
};

struct UpstageKnobLarge : rack::RoundKnob {
  UpstageKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/upstage/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct UpstagePort : rack::SVGPort {
  UpstagePort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/upstage/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct UpstageSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  UpstageSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/upstage/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/upstage/switch-2-high.svg")));
  }
};

struct UpstageWidget : public ModuleWidget {
  UpstageWidget(rack::Module *module) : ModuleWidget(module, 5, "res/upstage/panel.svg") {
    auto widget_right_edge = width();

    auto left_x = width()/4.f + 0.333333333f;
    auto center_x = widget_right_edge/2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_knob<UpstageKnobLarge>(Upstage::LEVEL_KNOB, {center_x, top_row_y + row*row_spacing});

    row++;
    install_input<UpstagePort>(Upstage::LEVEL_CV, {left_x, top_row_y + row*row_spacing});
    install_switch<UpstageSwitch2>(Upstage::LEVEL_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

    row++;
    install_switch<UpstageButtonNormal>(Upstage::WAIT_BUTTON, {left_x, top_row_y + row*row_spacing});
    install_switch<UpstageButtonNormal>(Upstage::TRIG_BUTTON, {right_x, top_row_y + row*row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input<UpstagePort>(Upstage::WAIT_IN, {left_x, top_row_y + row*row_spacing});

    row++;
    install_input<UpstagePort>(Upstage::TRIG_IN, {left_x, top_row_y + row*row_spacing});
    install_output<UpstagePort>(Upstage::TRIG_OUT, {right_x, top_row_y + row*row_spacing});

    row++;
    install_output<UpstagePort>(Upstage::ENVELOPE_OUT, {right_x, top_row_y + row*row_spacing});
  }
};
}
rack::Model *modelUpstage = rack::Model::create<DHE::Upstage, DHE::UpstageWidget>("DHE-Modules", "Upstage", "Upstage", rack::ENVELOPE_GENERATOR_TAG);
