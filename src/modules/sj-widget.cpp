#include <componentlibrary.hpp>

#include "sj-module.h"
#include "sj-widget.h"

namespace DHE {

SJWidget::SJWidget(rack::Module *module) : Widget(module, 4, "res/Swave.svg") {
  auto widget_right_edge{width()};

  auto left_x{5.5f};
  auto center_x{widget_right_edge/2.f};
  auto right_x{widget_right_edge - left_x};

  auto top_row_y{25.f};
  auto row_spacing{18.5f};


  auto row{0};
  install_knob<rack::RoundBlackKnob>(SJModule::SHAPE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_input<rack::PJ301MPort>(SJModule::SHAPE_CV, {left_x, top_row_y + row*row_spacing});
  install_switch<rack::CKSS>(SJModule::SHAPE_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

  row++;
  install_knob<rack::RoundSmallBlackKnob>(SJModule::TRIM_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 112.f;
  row_spacing = 15.f;

  row = 0;

  install_input<rack::PJ301MPort>(SJModule::SPORT_IN, {left_x, top_row_y + row*row_spacing});
  install_output<rack::PJ301MPort>(SJModule::SPORT_OUT, {right_x, top_row_y + row*row_spacing});
}
}