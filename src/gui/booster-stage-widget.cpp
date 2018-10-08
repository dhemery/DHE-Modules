#include <asset.hpp>
#include <componentlibrary.hpp>

#include "modules/booster-stage-module.h"
#include "booster-stage-widget.h"

namespace DHE {

struct BoosterStageButtonReverse : rack::SVGSwitch, rack::MomentarySwitch {
  BoosterStageButtonReverse() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/button-reverse-off.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/button-reverse-on.svg")));
  }
};

struct BoosterStageButtonNormal : rack::SVGSwitch, rack::MomentarySwitch {
  BoosterStageButtonNormal() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/button-normal-off.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/button-normal-on.svg")));
  }
};

struct BoosterStageKnobLarge : rack::RoundKnob {
  BoosterStageKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct BoosterStagePort : rack::SVGPort {
  BoosterStagePort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/booster-stage/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct BoosterStageSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  BoosterStageSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/switch-2-high.svg")));
  }
};

struct BoosterStageSwitch3 : rack::SVGSwitch, rack::ToggleSwitch {
  BoosterStageSwitch3() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/switch-3-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/switch-3-mid.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/booster-stage/switch-3-high.svg")));
  }
};

BoosterStageWidget::BoosterStageWidget(rack::Module *module) : ModuleWidget(module, 8, "res/booster-stage/panel.svg") {
  auto width = 8.f*5.08f;

  auto left_x = width/6.f + 0.3333333f;
  auto center_x = width/2.f;
  auto right_x = width - left_x;
  auto button_port_distance = 7.891f;
  auto center_left_x = left_x + button_port_distance;
  auto center_right_x = right_x - button_port_distance;

  auto top_row_y = 25.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_input<BoosterStagePort>(BoosterStageModule::LEVEL_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BoosterStageKnobLarge>(BoosterStageModule::LEVEL_KNOB, {center_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageSwitch2>(BoosterStageModule::LEVEL_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

  row++;
  install_input<BoosterStagePort>(BoosterStageModule::CURVE_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BoosterStageKnobLarge>(BoosterStageModule::CURVE_KNOB, {center_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageSwitch2>(BoosterStageModule::SHAPE_SWITCH, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<BoosterStagePort>(BoosterStageModule::DURATION_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BoosterStageKnobLarge>(BoosterStageModule::DURATION_KNOB, {center_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageSwitch3>(BoosterStageModule::DURATION_SWITCH, {right_x, top_row_y + row*row_spacing}, 2, 1);

  top_row_y = 82.f;
  row_spacing = 15.f;

  row = 0;
  install_input<BoosterStagePort>(BoosterStageModule::DEFER_IN, {left_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageButtonNormal>(BoosterStageModule::DEFER_BUTTON, {center_left_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageButtonReverse>(BoosterStageModule::ACTIVE_BUTTON, {center_right_x, top_row_y + row*row_spacing});
  install_output<BoosterStagePort>(BoosterStageModule::ACTIVE_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<BoosterStagePort>(BoosterStageModule::TRIGGER_IN, {left_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageButtonNormal>(BoosterStageModule::TRIGGER_BUTTON, {center_left_x, top_row_y + row*row_spacing});
  install_switch<BoosterStageButtonReverse>(BoosterStageModule::EOC_BUTTON, {center_right_x, top_row_y + row*row_spacing});
  install_output<BoosterStagePort>(BoosterStageModule::EOC_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<BoosterStagePort>(BoosterStageModule::ENVELOPE_IN, {left_x, top_row_y + row*row_spacing});
  install_output<BoosterStagePort>(BoosterStageModule::ENVELOPE_OUT, {right_x, top_row_y + row*row_spacing});
}
}
rack::Model *modelBoosterStage = rack::Model::create<DHE::BoosterStageModule, DHE::BoosterStageWidget>("DHE-Modules", "BoosterStage", "BoosterStage", rack::ENVELOPE_GENERATOR_TAG);
