#include <componentlibrary.hpp>
#include <plugin/dhe-modules.h>

#include "swave-module.h"
#include "swave-widget.h"

namespace DHE {

struct SwaveKnobLarge : rack::RoundKnob {
  SwaveKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/swave/knob-large.svg")));
    box.size = rack::Vec(39.f, 39.f);
  }
};

struct SwaveKnobSmall : rack::RoundKnob {
  SwaveKnobSmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/swave/knob-small.svg")));
//    box.size = mm2px(rack::Vec(9.f, 9.f));
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

SwaveWidget::SwaveWidget(rack::Module *module) : ModuleWidget(module, 4, "res/swave/panel.svg") {
  auto widget_right_edge = width();

  auto left_x = 5.3f;
  auto center_x = widget_right_edge/2.f;
  auto right_x = widget_right_edge - left_x;

  auto top_row_y = 25.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_knob<SwaveKnobLarge>(SwaveModule::CURVE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_input<SwavePort>(SwaveModule::CURVE_CV, {left_x, top_row_y + row*row_spacing});
  install_switch<SwaveSwitch2>(SwaveModule::SHAPE_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

  row++;
  install_knob<SwaveKnobSmall>(SwaveModule::TRIM_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 112.f;
  row_spacing = 15.f;

  row = 0;

  install_input<SwavePort>(SwaveModule::SWAVE_IN, {left_x, top_row_y + row*row_spacing});
  install_output<SwavePort>(SwaveModule::SWAVE_OUT, {right_x, top_row_y + row*row_spacing});
}
}
