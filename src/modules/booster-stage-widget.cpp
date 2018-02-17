#include <componentlibrary.hpp>

#include "booster-stage-module.h"
#include "booster-stage-widget.h"

namespace DHE {

BoosterStageWidget::BoosterStageWidget(rack::Module *module) : Widget(module, 8, "res/BoosterStage.svg") {
  auto widget_right_edge{this->box.size.x};

  auto top_knob_y{rack::mm2px(25.f)};
  auto knob_spacing{rack::mm2px(18.5f)};
  auto left_x{rack::mm2px(7.f)};
  auto right_x{widget_right_edge - left_x};
  auto center_x{widget_right_edge / 2.f};

  auto row_spacing{knob_spacing};

  auto row{0};
  install_input<rack::PJ301MPort>(BoosterStageModule::LEVEL_CV, left_x, top_knob_y + row*row_spacing);
  install_knob<rack::RoundBlackKnob>(BoosterStageModule::LEVEL_KNOB, center_x, top_knob_y + row*row_spacing);
  install_switch<rack::CKSS>(BoosterStageModule::LEVEL_SWITCH, right_x, top_knob_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(BoosterStageModule::SHAPE_CV, left_x, top_knob_y + row*row_spacing);
  install_knob<rack::RoundBlackKnob>(BoosterStageModule::SHAPE_KNOB, center_x, top_knob_y + row*row_spacing);
  install_switch<rack::CKSS>(BoosterStageModule::SHAPE_SWITCH, right_x, top_knob_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(BoosterStageModule::DURATION_CV, left_x, top_knob_y + row*row_spacing);
  install_knob<rack::RoundBlackKnob>(BoosterStageModule::DURATION_KNOB, center_x, top_knob_y + row*row_spacing);
  install_switch<rack::CKSSThree>(BoosterStageModule::DURATION_SWITCH, right_x, top_knob_y + row*row_spacing, 2, 1);

  auto top_port_y{rack::mm2px(82.f)};
  auto port_spacing{rack::mm2px(15.f)};

  row_spacing = port_spacing;

  row = 0;
  install_input<rack::PJ301MPort>(BoosterStageModule::DEFER_INPUT, left_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(BoosterStageModule::ACTIVE_OUTPUT, right_x, top_port_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(BoosterStageModule::TRIG_INPUT, left_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(BoosterStageModule::EOC_OUTPUT, right_x, top_port_y + row*row_spacing);

  row++;
  install_input<rack::PJ301MPort>(BoosterStageModule::IN_INPUT, left_x, top_port_y + row*row_spacing);
  install_output<rack::PJ301MPort>(BoosterStageModule::OUT_OUTPUT, right_x, top_port_y + row*row_spacing);
}
}