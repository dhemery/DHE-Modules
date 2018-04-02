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
  auto coefficient_knob_range = Range{-2.f, 2.f};
  auto gain_knob_range = Range{0.f, 2.f};
  auto widget_right_edge = width();

  auto left_x = width()/4.f + 0.333333f;
  auto right_x = widget_right_edge - left_x;

  auto top_row_y = 25.f;
  auto row_spacing = 16.f;

  auto row = 0;
  install_input<CubicPort>(CubicModule::A_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::A_KNOB, {right_x, top_row_y + row*row_spacing}, coefficient_knob_range);

  row++;
  install_input<CubicPort>(CubicModule::B_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::B_KNOB, {right_x, top_row_y + row*row_spacing}, coefficient_knob_range);

  row++;
  install_input<CubicPort>(CubicModule::C_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::C_KNOB, {right_x, top_row_y + row*row_spacing}, coefficient_knob_range, 1.f);

  row++;
  install_input<CubicPort>(CubicModule::D_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<CubicKnobMedium>(CubicModule::D_KNOB, {right_x, top_row_y + row*row_spacing}, coefficient_knob_range);

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;

  row++;
  install_knob<CubicKnobMedium>(CubicModule::INPUT_GAIN_KNOB, {left_x, top_row_y + row*row_spacing}, gain_knob_range, 1.f);
  install_knob<CubicKnobMedium>(CubicModule::OUTPUT_GAIN_KNOB, {right_x, top_row_y + row*row_spacing}, gain_knob_range, 1.f);

  row++;
  install_input<CubicPort>(CubicModule::X, {left_x, top_row_y + row*row_spacing});
  install_output<CubicPort>(CubicModule::Y, {right_x, top_row_y + row*row_spacing});
}
}
