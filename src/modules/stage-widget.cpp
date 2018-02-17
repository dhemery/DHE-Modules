#include <componentlibrary.hpp>

#include "stage-module.h"
#include "stage-widget.h"

namespace DHE {

StageWidget::StageWidget(rack::Module *module) : Widget(module, 4, "res/Stage.svg") {
  auto widget_right_edge{this->box.size.x};

  auto top_knob_y{75.0f};
  auto knob_spacing{55.0f};
  auto knob_x{widget_right_edge/2.0f};

  auto row_spacing{knob_spacing};

  auto row{0};
  install_knob<rack::RoundBlackKnob>(StageModule::LEVEL_KNOB, knob_x, top_knob_y + row*row_spacing);

  row++;
  install_knob<rack::RoundBlackKnob>(StageModule::SHAPE_KNOB, knob_x, top_knob_y + row*row_spacing);

  row++;
  install_knob<rack::RoundBlackKnob>(StageModule::DURATION_KNOB, knob_x, top_knob_y + row*row_spacing);

  auto top_port_y{245.0f};
  auto port_spacing{43.0f};
  auto left_port_x{16.0f};
  auto right_port_x{widget_right_edge - left_port_x};

  row_spacing = port_spacing;

  row = 0;
  install_input<rack::PJ301MPort>(StageModule::DEFER_INPUT, left_port_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(StageModule::ACTIVE_OUTPUT, right_port_x, top_port_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(StageModule::TRIG_INPUT, left_port_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(StageModule::EOC_OUTPUT, right_port_x, top_port_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(StageModule::IN_INPUT, left_port_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(StageModule::OUT_OUTPUT, right_port_x, top_port_y + row*row_spacing);
}
}