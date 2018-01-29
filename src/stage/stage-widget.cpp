#include "stage.h"
#include "stage-widget.h"

namespace DHE {

StageWidget::StageWidget() : Widget(new Stage(), 4, "res/Stage.svg") {
  float widget_right_edge = this->box.size.x;

  float top_knob_y = 75.0f;
  float knob_spacing = 55.0f;
  float knob_x = widget_right_edge / 2.0f;
  install_param<rack::RoundBlackKnob>(Stage::LEVEL_KNOB, knob_x, top_knob_y);
  install_param<rack::RoundBlackKnob>(Stage::SHAPE_KNOB, knob_x, top_knob_y +  knob_spacing);
  install_param<rack::RoundBlackKnob>(Stage::DURATION_KNOB, knob_x, top_knob_y + 2 * knob_spacing);

  float left_port_x = 16.0f;
  float top_port_y = 245.0f;
  float port_spacing = 43.0f;
  install_input<rack::PJ301MPort>(Stage::DEFER_GATE_IN, left_port_x, top_port_y);
  install_input<rack::PJ301MPort>(Stage::TRIGGER_IN, left_port_x, top_port_y + port_spacing);
  install_input<rack::PJ301MPort>(Stage::STAGE_IN, left_port_x, top_port_y + 2 * port_spacing);

  float right_port_x = widget_right_edge - left_port_x;
  install_output<rack::PJ301MPort>(Stage::ACTIVE_GATE_OUT, right_port_x, top_port_y);
  install_output<rack::PJ301MPort>(Stage::EOC_TRIGGER_OUT, right_port_x, top_port_y + port_spacing);
  install_output<rack::PJ301MPort>(Stage::STAGE_OUT, right_port_x, top_port_y + 2 * port_spacing);

  install_screws(13.0f, 7.5f);
}
}