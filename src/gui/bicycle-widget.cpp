#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/bicycle-module.h"
#include "bicycle-widget.h"

namespace DHE {

struct BicycleKnobTiny : rack::RoundKnob {
  BicycleKnobTiny() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/bicycle/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct BicycleKnobSmall : rack::RoundKnob {
  BicycleKnobSmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/bicycle/knob-small.svg")));
    shadow->opacity = 0.f;
  }
};

struct BicycleKnobLarge: rack::RoundKnob {
  BicycleKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/bicycle/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct BicycleSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  BicycleSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/bicycle/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/bicycle/switch-2-high.svg")));
  }
};

struct BicyclePort : rack::SVGPort {
  BicyclePort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/bicycle/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

BicycleWidget::BicycleWidget(rack::Module *module) : ModuleWidget(module, 11, "res/bicycle/panel.svg") {
  auto width = 11.f*5.08f;

  auto left_x = width/7.f;
  auto right_x = width - left_x;
  auto left_center_x = (right_x - left_x)/3.f + left_x;
  auto right_center_x = width - left_center_x;

  auto top_row_y = 22.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_input<BicyclePort>(BicycleModule::BASE_RADIUS_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobTiny>(BicycleModule::BASE_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobLarge>(BicycleModule::BASE_RADIUS_KNOB, {right_center_x, top_row_y + row*row_spacing});

  row++;
  install_input<BicyclePort>(BicycleModule::ROLL_RADIUS_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobTiny>(BicycleModule::ROLL_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobLarge>(BicycleModule::ROLL_RADIUS_KNOB, {right_center_x, top_row_y + row*row_spacing});
  install_switch<BicycleSwitch2>(BicycleModule::ROLL_POSITION_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 0);

  row++;
  install_input<BicyclePort>(BicycleModule::POLE_LENGTH_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobTiny>(BicycleModule::POLE_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobLarge>(BicycleModule::POLE_LENGTH_KNOB, {right_center_x, top_row_y + row*row_spacing});

  row++;
  install_input<BicyclePort>(BicycleModule::SPEED_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobTiny>(BicycleModule::SPEED_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobLarge>(BicycleModule::SPEED_KNOB, {right_center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;
  row = 0;

  auto default_gain = BicycleModule::gain_range().normalize(1.f);
  row++;
  install_input<BicyclePort>(BicycleModule::X_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobSmall>(BicycleModule::X_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<BicycleSwitch2>(BicycleModule::X_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<BicyclePort>(BicycleModule::X_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<BicyclePort>(BicycleModule::Y_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<BicycleKnobSmall>(BicycleModule::Y_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<BicycleSwitch2>(BicycleModule::Y_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<BicyclePort>(BicycleModule::Y_OUT, {right_x, top_row_y + row*row_spacing});
}
}
