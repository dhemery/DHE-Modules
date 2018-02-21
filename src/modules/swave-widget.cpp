#include <componentlibrary.hpp>

#include "swave-module.h"
#include "swave-widget.h"

namespace DHE {

SwaveWidget::SwaveWidget(rack::Module *module) : Widget(module, 4, "res/Swave.svg") {
  auto widget_right_edge{width()};

  auto left_x{5.5f};
  auto center_x{widget_right_edge/2.f};
  auto right_x{widget_right_edge - left_x};

  auto top_row_y{25.f};
  auto row_spacing{18.5f};


  auto row{0};
  install_knob<rack::RoundBlackKnob>(SwaveModule::SHAPE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_input<rack::PJ301MPort>(SwaveModule::SHAPE_CV, {left_x, top_row_y + row*row_spacing});
  install_switch<rack::CKSS>(SwaveModule::SHAPE_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

  row++;
  install_knob<rack::RoundSmallBlackKnob>(SwaveModule::TRIM_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 112.f;
  row_spacing = 15.f;

  row = 0;

  install_input<rack::PJ301MPort>(SwaveModule::SPORT_IN, {left_x, top_row_y + row*row_spacing});
  install_output<rack::PJ301MPort>(SwaveModule::SPORT_OUT, {right_x, top_row_y + row*row_spacing});
}
}