#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/cubic-module.h"
#include "cubic-widget.h"

namespace DHE {

struct CubicKnobLarge : rack::RoundKnob {
  CubicKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/Cubic/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct CubicPort : rack::SVGPort {
  CubicPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/Cubic/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

CubicWidget::CubicWidget(rack::Module *module) : ModuleWidget(module, 4, "res/Cubic/panel.svg") {
  auto widget_right_edge = width();

  auto center_x = widget_right_edge/2.f;

  auto top_row_y = 25.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_knob<CubicKnobLarge>(CubicModule::CUBED_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<CubicKnobLarge>(CubicModule::SQUARED_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<CubicKnobLarge>(CubicModule::SCALE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<CubicKnobLarge>(CubicModule::OFFSET_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;

  row++;
  install_input<CubicPort>(CubicModule::X, {center_x, top_row_y + row*row_spacing});

  row++;
  install_output<CubicPort>(CubicModule::Y, {center_x, top_row_y + row*row_spacing});
}
}
