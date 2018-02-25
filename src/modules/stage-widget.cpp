#include <componentlibrary.hpp>
#include <plugin/dhe-modules.h>

#include "stage-module.h"
#include "stage-widget.h"

namespace DHE {

struct StagePort : rack::SVGPort {
  StagePort() {
    background->svg = rack::SVG::load(rack::assetPlugin(plugin, "res/stage/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct StageKnobLarge : rack::RoundKnob {
  StageKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/stage/knob-large.svg")));
    box.size = rack::Vec(39.f, 39.f);
  }
};

StageWidget::StageWidget(rack::Module *module) : ModuleWidget(module, 4, "res/stage/panel.svg") {
  auto widget_right_edge{width()};

  auto left_x{16.f / 3.f};
  auto center_x{widget_right_edge/2.f};
  auto right_x{widget_right_edge - left_x};

  auto top_row_y{25.f};
  auto row_spacing{18.5f};

  auto row{0};
  install_knob<StageKnobLarge>(StageModule::LEVEL_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<StageKnobLarge>(StageModule::SHAPE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<StageKnobLarge>(StageModule::DURATION_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;
  install_input<StagePort>(StageModule::DEFER_IN, {left_x, top_row_y + row*row_spacing});
  install_output<StagePort>(StageModule::ACTIVE_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<StagePort>(StageModule::TRIG_IN, {left_x, top_row_y + row*row_spacing});
  install_output<StagePort>(StageModule::EOC_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<StagePort>(StageModule::STAGE_IN, {left_x, top_row_y + row*row_spacing});
  install_output<StagePort>(StageModule::STAGE_OUT, {right_x, top_row_y + row*row_spacing});
}
}