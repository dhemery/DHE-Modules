#include <componentlibrary.hpp>

#include "stage-module.h"
#include "stage-widget.h"

namespace DHE {

StageWidget::StageWidget(rack::Module *module) : Widget(module, 4, "res/Stage.svg") {
  auto widget_right_edge{this->box.size.x};

  auto left_x{rack::mm2px(7.f)};
  auto center_x{widget_right_edge / 2.f};
  auto right_x{widget_right_edge - left_x};

  auto top_row_y{rack::mm2px(25.f)};
  auto row_spacing{rack::mm2px(18.5f)};

  auto row{0};
  install_knob<rack::RoundBlackKnob>(StageModule::LEVEL_KNOB, center_x, top_row_y + row*row_spacing);

  row++;
  install_knob<rack::RoundBlackKnob>(StageModule::SHAPE_KNOB, center_x, top_row_y + row*row_spacing);

  row++;
  install_knob<rack::RoundBlackKnob>(StageModule::DURATION_KNOB, center_x, top_row_y + row*row_spacing);

  left_x = rack::mm2px(5.5f);
  right_x = widget_right_edge - left_x;
  top_row_y = rack::mm2px(82.f);
  row_spacing = rack::mm2px(15.f);

  row = 0;
  install_input<rack::PJ301MPort>(StageModule::DEFER_INPUT, left_x, top_row_y + row*row_spacing);
  install_output<rack::PJ301MPort>(StageModule::ACTIVE_OUTPUT, right_x, top_row_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(StageModule::TRIG_INPUT, left_x, top_row_y + row*row_spacing);
  install_output<rack::PJ301MPort>(StageModule::EOC_OUTPUT, right_x, top_row_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(StageModule::IN_INPUT, left_x, top_row_y + row*row_spacing);
  install_output<rack::PJ301MPort>(StageModule::OUT_OUTPUT, right_x, top_row_y + row*row_spacing);
}
}