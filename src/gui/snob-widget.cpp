#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "gui/snob-widget.h"
#include "modules/snob-module.h"

namespace DHE {

struct SnobKnobLarge : rack::RoundKnob {
  SnobKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/snob/knob-large.svg")));
    box.size = rack::Vec(39.f, 39.f);
  }
};

struct SnobPort : rack::SVGPort {
  SnobPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/snob/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct SnobSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  SnobSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/snob/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/snob/switch-2-high.svg")));
  }
};

SnobWidget::SnobWidget(rack::Module *module) : ModuleWidget(module, 4, "res/snob/panel.svg") {
  auto widget_right_edge = width();

  auto left_x = width()/4.f + 0.333333333f;
  auto center_x = widget_right_edge/2.f;
  auto right_x = widget_right_edge - left_x;

  auto top_row_y = 25.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_knob<SnobKnobLarge>(SnobModule::LEVEL_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_input<SnobPort>(SnobModule::LEVEL_CV, {left_x, top_row_y + row*row_spacing});
  install_switch<SnobSwitch2>(SnobModule::LEVEL_SWITCH, {right_x, top_row_y + row*row_spacing});

  row++;
  install_knob<SnobKnobLarge>(SnobModule::CURVE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_input<SnobPort>(SnobModule::CURVE_CV, {left_x, top_row_y + row*row_spacing});
  install_switch<SnobSwitch2>(SnobModule::SHAPE_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;
  row++;

  row++;
  install_output<SnobPort>(SnobModule::SNOB_OUT, {center_x, top_row_y + row*row_spacing});
}
}
