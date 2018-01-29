#include "upstage.h"
#include "upstage-widget.h"

namespace DHE {

UpstageWidget::UpstageWidget() : Widget(new Upstage(), 4, "res/Upstage.svg") {
  float widget_right_edge = this->box.size.x;
  float center_x = widget_right_edge / 2.0f;

  float upper_top_y = 75.0f;
  float upper_spacing = 55.0f;
  float left_x = 16.0f;
  float right_x = widget_right_edge - left_x;

  install_param<rack::RoundBlackKnob>(Upstage::LEVEL_KNOB, center_x, upper_top_y);
  install_button<rack::LEDButton>(Upstage::WAIT_BUTTON, left_x, upper_top_y + upper_spacing);
  install_button<rack::LEDButton>(Upstage::TRIG_BUTTON, right_x, upper_top_y + upper_spacing);


  float lower_top_y = 245.0f;
  float lower_spacing = 43.0f;

  install_input<rack::PJ301MPort>(Upstage::WAIT_GATE_IN, left_x, lower_top_y);

  install_input<rack::PJ301MPort>(Upstage::TRIG_IN, left_x, lower_top_y + lower_spacing);
  install_output<rack::PJ301MPort>(Upstage::TRIG_OUT, right_x, lower_top_y + lower_spacing);

  install_output<rack::PJ301MPort>(Upstage::ENVELOPE_OUT, right_x, lower_top_y + 2 * lower_spacing);

  install_screws(13.0f, 7.5f);
}
}