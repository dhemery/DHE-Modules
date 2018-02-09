#include "modules/stage.h"
#include "stage-widget.h"

namespace DHE {

StageWidget::StageWidget() : Widget(new Stage(&rack::engineGetSampleTime), 4, "res/Stage.svg") {
  auto widget_right_edge{this->box.size.x};

  auto top_knob_y{75.0f};
  auto knob_spacing{55.0f};
  auto knob_x{widget_right_edge/2.0f};

  auto row_spacing{knob_spacing};

  auto row{0};
  install_param<rack::RoundBlackKnob>(Stage::LEVEL_KNOB, knob_x, top_knob_y + row*row_spacing);

  row++;
  install_param<rack::RoundBlackKnob>(Stage::SHAPE_KNOB, knob_x, top_knob_y + row*row_spacing);

  row++;
  install_param<rack::RoundBlackKnob>(Stage::DURATION_KNOB, knob_x, top_knob_y + row*row_spacing);

  auto top_port_y{245.0f};
  auto port_spacing{43.0f};
  auto left_port_x{16.0f};
  auto right_port_x{widget_right_edge - left_port_x};

  row_spacing = port_spacing;

  row = 0;
  install_input<rack::PJ301MPort>(Stage::DEFER_INPUT, left_port_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(Stage::ACTIVE_OUTPUT, right_port_x, top_port_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(Stage::TRIG_INPUT, left_port_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(Stage::EOC_OUTPUT, right_port_x, top_port_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(Stage::IN_INPUT, left_port_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(Stage::OUT_OUTPUT, right_port_x, top_port_y + row*row_spacing);
}
}