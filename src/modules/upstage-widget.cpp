#include <componentlibrary.hpp>

#include "upstage-module.h"
#include "upstage-widget.h"

namespace DHE {

UpstageWidget::UpstageWidget(rack::Module *module) : Widget(module, 4, "res/Upstage.svg") {
  auto widget_right_edge{this->box.size.x};
  auto center_x{widget_right_edge/2.0f};

  auto upper_top_y{75.0f};
  auto upper_spacing{55.0f};
  auto left_x{16.0f};
  auto right_x{widget_right_edge - left_x};

  auto top_y{upper_top_y};
  auto row_spacing{upper_spacing};

  auto row{0};
  install_param<rack::RoundBlackKnob>(UpstageModule::LEVEL_KNOB, center_x, top_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(UpstageModule::LEVEL_CV_INPUT, center_x, top_y + row*row_spacing);

  row++;
  install_button<rack::LEDButton>(UpstageModule::WAIT_BUTTON, left_x, top_y + row*row_spacing);
  install_light<rack::MediumLight<rack::GreenLight>>(UpstageModule::WAIT_BUTTON_LIGHT, left_x, top_y + row*row_spacing);
  install_button<rack::LEDButton>(UpstageModule::TRIG_BUTTON, right_x, upper_top_y + row*row_spacing);
  install_light<rack::MediumLight<rack::GreenLight>>(UpstageModule::TRIG_BUTTON_LIGHT, right_x, top_y + row*row_spacing);

  auto lower_top_y{245.0f};
  auto lower_spacing{43.0f};

  top_y = lower_top_y;
  row_spacing = lower_spacing;

  row = 0;
  install_input<rack::PJ301MPort>(UpstageModule::WAIT_INPUT, left_x, top_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(UpstageModule::TRIG_INPUT, left_x, top_y + row*row_spacing);
  install_output<rack::PJ301MPort>(UpstageModule::TRIG_OUTPUT, right_x, top_y + row*row_spacing);

  row++;
  install_output<rack::PJ301MPort>(UpstageModule::OUT_OUTPUT, right_x, top_y + row*row_spacing);
}
}