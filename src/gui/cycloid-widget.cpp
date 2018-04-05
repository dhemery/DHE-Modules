#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/cycloid-module.h"
#include "cycloid-widget.h"

namespace DHE {

struct CycloidKnobLarge : rack::RoundKnob {
  CycloidKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/cycloid/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct CycloidPort : rack::SVGPort {
  CycloidPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/cycloid/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

CycloidWidget::CycloidWidget(rack::Module *module) : ModuleWidget(module, 5, "res/cycloid/panel.svg") {
  auto widget_right_edge = width();

  auto center_x = width()/2.f;
  auto left_x = width()/4.f + 0.333333f;
  auto right_x = widget_right_edge - left_x;

  auto top_row_y = 25.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_knob<CycloidKnobLarge>(CycloidModule::FIXED_RADIUS_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<CycloidKnobLarge>(CycloidModule::MOVING_RADIUS_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<CycloidKnobLarge>(CycloidModule::ARM_LENGTH_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;
  row = 0;

  row++;

  row++;
  install_output<CycloidPort>(CycloidModule::X_OUT, {left_x, top_row_y + row*row_spacing});
  install_output<CycloidPort>(CycloidModule::Y_OUT, {right_x, top_row_y + row*row_spacing});
}
}
