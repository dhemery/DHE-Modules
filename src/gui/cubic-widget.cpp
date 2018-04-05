#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/cubic-module.h"
#include "cubic-widget.h"

namespace DHE {

struct CubicKnobsmall : rack::RoundKnob {
  CubicKnobsmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/cubic/knob-small.svg")));
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

  auto top_row_y = 20.f;
  auto row_spacing = 15.f;

  auto row = 0;
  install_input<CubicPort>(CubicModule::X3_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobsmall>(CubicModule::X3_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::X2_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobsmall>(CubicModule::X2_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::X1_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobsmall>(CubicModule::X1_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::X0_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobsmall>(CubicModule::X0_KNOB, {right_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row = 0;
  install_knob<CubicKnobsmall>(CubicModule::INPUT_GAIN_KNOB, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobsmall>(CubicModule::OUTPUT_GAIN_KNOB, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::INPUT_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_input<CubicPort>(CubicModule::OUTPUT_GAIN_CV, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<CubicPort>(CubicModule::IN, {left_x, top_row_y + row*row_spacing});
  install_output<CubicPort>(CubicModule::OUT, {right_x, top_row_y + row*row_spacing});
}
}
