#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/xynchrotron-module.h"
#include "xynchrotron-widget.h"

namespace DHE {

struct XynchrotronKnobTiny : rack::RoundKnob {
  XynchrotronKnobTiny() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xyclotron/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct XynchrotronKnobSmall : rack::RoundKnob {
  XynchrotronKnobSmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xynchrotron/knob-small.svg")));
    shadow->opacity = 0.f;
  }
};

struct XynchrotronKnobLarge: rack::RoundKnob {
  XynchrotronKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xynchrotron/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct XynchrotronSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  XynchrotronSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xynchrotron/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xynchrotron/switch-2-high.svg")));
  }
};

struct XynchrotronPort : rack::SVGPort {
  XynchrotronPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/xynchrotron/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

XynchrotronWidget::XynchrotronWidget(rack::Module *module) : ModuleWidget(module, 11, "res/xynchrotron/panel.svg") {
  auto width = 11.f*5.08f;

  auto left_x = width/7.f;
  auto right_x = width - left_x;
  auto left_center_x = (right_x - left_x)/3.f + left_x;
  auto right_center_x = width - left_center_x;

  auto top_row_y = 22.f;
  auto row_spacing = 18.5f;

  auto row = 0;
  install_input<XynchrotronPort>(XynchrotronModule::ROCK_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobTiny>(XynchrotronModule::ROCK_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobLarge>(XynchrotronModule::ROCK_KNOB, {right_center_x, top_row_y + row*row_spacing});

  row++;
  install_input<XynchrotronPort>(XynchrotronModule::ROLL_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobTiny>(XynchrotronModule::ROLL_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobLarge>(XynchrotronModule::ROLL_KNOB, {right_center_x, top_row_y + row*row_spacing});
  install_switch<XynchrotronSwitch2>(XynchrotronModule::ROLL_POSITION_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 0);

  row++;
  install_input<XynchrotronPort>(XynchrotronModule::CURL_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobTiny>(XynchrotronModule::CURL_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobLarge>(XynchrotronModule::CURL_KNOB, {right_center_x, top_row_y + row*row_spacing});

  row++;
  install_input<XynchrotronPort>(XynchrotronModule::ZING_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobTiny>(XynchrotronModule::ZING_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobLarge>(XynchrotronModule::ZING_KNOB, {right_center_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;
  row = 0;

  auto default_gain = XynchrotronModule::gain_range().normalize(1.f);
  row++;
  install_input<XynchrotronPort>(XynchrotronModule::X_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobSmall>(XynchrotronModule::X_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<XynchrotronSwitch2>(XynchrotronModule::X_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<XynchrotronPort>(XynchrotronModule::X_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<XynchrotronPort>(XynchrotronModule::Y_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XynchrotronKnobSmall>(XynchrotronModule::Y_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<XynchrotronSwitch2>(XynchrotronModule::Y_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<XynchrotronPort>(XynchrotronModule::Y_OUT, {right_x, top_row_y + row*row_spacing});
}
}
