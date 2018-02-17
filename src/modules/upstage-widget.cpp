#include <componentlibrary.hpp>

#include "upstage-module.h"
#include "upstage-widget.h"

namespace DHE {

UpstageWidget::UpstageWidget(rack::Module *module) : Widget(module, 4, "res/Upstage.svg") {
  auto widget_right_edge{width()};

  auto left_x{5.5f};
  auto center_x{widget_right_edge / 2.f};
  auto right_x{widget_right_edge - left_x};

  auto top_row_y{25.f};
  auto row_spacing{18.5f};

  auto row{0};
  install_knob<rack::RoundBlackKnob>(UpstageModule::LEVEL_KNOB, center_x, top_row_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(UpstageModule::LEVEL_CV_INPUT, center_x, top_row_y + row*row_spacing);

  row++;
  install_button<rack::LEDButton>(UpstageModule::WAIT_BUTTON, left_x, top_row_y + row*row_spacing);
  install_light<rack::MediumLight<rack::GreenLight>>(UpstageModule::WAIT_BUTTON_LIGHT, left_x, top_row_y + row*row_spacing);
  install_button<rack::LEDButton>(UpstageModule::TRIG_BUTTON, right_x, top_row_y + row*row_spacing);
  install_light<rack::MediumLight<rack::GreenLight>>(UpstageModule::TRIG_BUTTON_LIGHT, right_x, top_row_y + row*row_spacing);

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;
  install_input<rack::PJ301MPort>(UpstageModule::WAIT_INPUT, left_x, top_row_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(UpstageModule::TRIG_INPUT, left_x, top_row_y + row*row_spacing);
  install_output<rack::PJ301MPort>(UpstageModule::TRIG_OUTPUT, right_x, top_row_y + row*row_spacing);

  row++;
  install_output<rack::PJ301MPort>(UpstageModule::OUT_OUTPUT, right_x, top_row_y + row*row_spacing);
}
}