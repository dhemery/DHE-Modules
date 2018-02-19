#include <componentlibrary.hpp>

#include "stage-module.h"
#include "stage-widget.h"

namespace DHE {

StageWidget::StageWidget(rack::Module *module) : Widget(module, 4, "res/Stage.svg") {
  auto widget_right_edge{width()};

  auto left_x{5.5f};
  auto center_x{widget_right_edge/2.f};
  auto right_x{widget_right_edge - left_x};

  auto top_row_y{25.f};
  auto row_spacing{18.5f};

  auto row{0};
  install_knob<rack::RoundBlackKnob>(StageModule::LEVEL_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<rack::RoundBlackKnob>(StageModule::SHAPE_KNOB, {center_x, top_row_y + row*row_spacing});

  row++;
  install_knob<rack::RoundBlackKnob>(StageModule::DURATION_KNOB, {center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;
  install_input<rack::PJ301MPort>(StageModule::DEFER_IN, {left_x, top_row_y + row*row_spacing});
  install_output<rack::PJ301MPort>(StageModule::ACTIVE_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<rack::PJ301MPort>(StageModule::TRIG_IN, {left_x, top_row_y + row*row_spacing});
  install_output<rack::PJ301MPort>(StageModule::EOC_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<rack::PJ301MPort>(StageModule::STAGE_IN, {left_x, top_row_y + row*row_spacing});
  install_output<rack::PJ301MPort>(StageModule::STAGE_OUT, {right_x, top_row_y + row*row_spacing});
}
}