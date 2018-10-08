#include <algorithm>
#include <componentlibrary.hpp>
#include <engine.hpp>

#include "gui/module-widget.h"
#include "modules/module.h"
#include "plugin/dhe-modules.h"
#include "util/controls.h"
#include "util/range.h"

namespace DHE {

struct Swave : Module {
  Swave() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  bool is_s_taper() const {
    return param(SHAPE_SWITCH) > 0.5f;
  }

  void step() override {
    outputs[SWAVE_OUT].value = to_signal(taper(to_phase(swave_in())));
  }

  float swave_in() const { return input(SWAVE_IN); }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
  }

  float to_signal(float phase) const {
    return BIPOLAR_SIGNAL_RANGE.scale(phase);
  }

  float to_phase(float signal) const {
    return BIPOLAR_SIGNAL_RANGE.normalize(signal);
  }

  enum ParameterIds {
    CURVE_KNOB,
    SHAPE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    CURVE_CV,
    SWAVE_IN,
    INPUT_COUNT
  };
  enum OutputIds {
    SWAVE_OUT,
    OUTPUT_COUNT
  };
};

struct SwaveKnobLarge : rack::RoundKnob {
  SwaveKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/swave/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct SwavePort : rack::SVGPort {
  SwavePort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/swave/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct SwaveSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  SwaveSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/swave/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/swave/switch-2-high.svg")));
  }
};

struct SwaveWidget : public ModuleWidget {
  SwaveWidget(rack::Module *module) : ModuleWidget(module, 4, "res/swave/panel.svg") {
    auto widget_right_edge = width();

    auto center_x = widget_right_edge/2.f;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_switch<SwaveSwitch2>(Swave::SHAPE_SWITCH, {center_x, top_row_y + row*row_spacing}, 1, 1);

    row++;
    install_knob<SwaveKnobLarge>(Swave::CURVE_KNOB, {center_x, top_row_y + row*row_spacing});

    row++;
    install_input<SwavePort>(Swave::CURVE_CV, {center_x, top_row_y + row*row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;

    row++;
    install_input<SwavePort>(Swave::SWAVE_IN, {center_x, top_row_y + row*row_spacing});

    row++;
    install_output<SwavePort>(Swave::SWAVE_OUT, {center_x, top_row_y + row*row_spacing});
  }
};
}
rack::Model *modelSwave = rack::Model::create<DHE::Swave, DHE::SwaveWidget>("DHE-Modules", "Swave", "Swave", rack::WAVESHAPER_TAG);
