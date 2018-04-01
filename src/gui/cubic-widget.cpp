#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/cubic-module.h"
#include "cubic-widget.h"

namespace DHE {

struct CubicKnobMedium : rack::RoundKnob {
  CubicKnobMedium() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/cubic/knob-medium.svg")));
    shadow->opacity = 0.f;
  }
};

struct CubicPort : rack::SVGPort {
  CubicPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/cubic/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

CubicWidget::CubicWidget(rack::Module *module) : ModuleWidget(module, 5, "res/cubic/panel.svg") {
  auto widget_right_edge = width();

  auto left_x = width()/4.f + 0.333333f;
  auto right_x = widget_right_edge - left_x;

  auto top_row_y = 25.f;
  auto row_spacing = 16.f;

  auto row = 0;
  install_input<CubicPort>(CubicModule::CUBED_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::CUBED_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::SQUARED_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::SQUARED_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::SCALE_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::SCALE_KNOB, {right_x, top_row_y + row*row_spacing}, 0.75f);

  row++;
  install_input<CubicPort>(CubicModule::OFFSET_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::OFFSET_KNOB, {right_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;

  row++;
  install_knob<CubicKnobMedium>(CubicModule::INPUT_GAIN_KNOB, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::OUTPUT_GAIN_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::X, {left_x, top_row_y + row*row_spacing});
  install_output<CubicPort>(CubicModule::Y, {right_x, top_row_y + row*row_spacing});
}
}
