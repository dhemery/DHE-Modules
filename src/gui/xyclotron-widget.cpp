#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/xyclotron-module.h"
#include "xyclotron-widget.h"

namespace DHE {

struct XyclotronKnobTiny : rack::RoundKnob {
  XyclotronKnobTiny() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xyclotron/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct XyclotronKnobSmall : rack::RoundKnob {
  XyclotronKnobSmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xyclotron/knob-small.svg")));
    shadow->opacity = 0.f;
  }
};

struct XyclotronKnobLarge: rack::RoundKnob {
  XyclotronKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xyclotron/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct XyclotronSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  XyclotronSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xyclotron/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xyclotron/switch-2-high.svg")));
  }
};

struct XyclotronPort : rack::SVGPort {
  XyclotronPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/xyclotron/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

XyclotronWidget::XyclotronWidget(rack::Module *module) : ModuleWidget(module, 11, "res/xyclotron/panel.svg") {
  auto width = 11.f*5.08f;

  auto left_x = width/7.f;
  auto right_x = width - left_x;
  auto left_center_x = (right_x - left_x)/3.f + left_x;
  auto right_center_x = width - left_center_x;

  auto top_row_y = 22.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_input<XyclotronPort>(BicycleModule::ROCK_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobTiny>(BicycleModule::ROCK_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobLarge>(BicycleModule::ROCK_KNOB, {right_center_x, top_row_y + row*row_spacing});

  row++;
  install_input<XyclotronPort>(BicycleModule::ROLL_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobTiny>(BicycleModule::ROLL_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobLarge>(BicycleModule::ROLL_KNOB, {right_center_x, top_row_y + row*row_spacing});
  install_switch<XyclotronSwitch2>(BicycleModule::ROLL_POSITION_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 0);

  row++;
  install_input<XyclotronPort>(BicycleModule::CURL_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobTiny>(BicycleModule::CURL_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobLarge>(BicycleModule::CURL_KNOB, {right_center_x, top_row_y + row*row_spacing});

  row++;
  install_input<XyclotronPort>(BicycleModule::HASTE_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobTiny>(BicycleModule::HASTE_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobLarge>(BicycleModule::HASTE_KNOB, {right_center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;
  row = 0;

  auto default_gain = BicycleModule::gain_range().normalize(1.f);
  row++;
  install_input<XyclotronPort>(BicycleModule::X_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobSmall>(BicycleModule::X_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<XyclotronSwitch2>(BicycleModule::X_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<XyclotronPort>(BicycleModule::X_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<XyclotronPort>(BicycleModule::Y_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XyclotronKnobSmall>(BicycleModule::Y_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<XyclotronSwitch2>(BicycleModule::Y_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<XyclotronPort>(BicycleModule::Y_OUT, {right_x, top_row_y + row*row_spacing});
}
}
