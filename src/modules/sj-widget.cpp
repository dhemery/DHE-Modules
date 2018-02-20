#include <componentlibrary.hpp>

#include "sj-module.h"
#include "sj-widget.h"

namespace DHE {

SJWidget::SJWidget(rack::Module *module) : Widget(module, 2, "res/SJ.svg") {
  auto widget_right_edge{width()};

  auto center_x{widget_right_edge/2.f};

  auto top_row_y{20.f};
  auto row_spacing{18.5f};

  auto row{0};
  install_knob<rack::Trimpot>(SJModule::SHAPE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_switch<rack::CKSS>(SJModule::SHAPE_SWITCH, {center_x, top_row_y + row*row_spacing}, 1, 1);

  row++;
  install_input<rack::PJ301MPort>(SJModule::SHAPE_CV, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<rack::Trimpot>(SJModule::TRIM_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;

  row++;
  install_input<rack::PJ301MPort>(SJModule::SPORT_IN, {center_x, top_row_y + row*row_spacing});

  row++;
  install_output<rack::PJ301MPort>(SJModule::SPORT_OUT, {center_x, top_row_y + row*row_spacing});
}
}