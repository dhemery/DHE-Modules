#include <componentlibrary.hpp>

#include "plugin/dhe-modules.h"
#include "modules/xycloid-module.h"
#include "xycloid-widget.h"

namespace DHE {

struct XycloidButtonNormal : rack::SVGSwitch, rack::MomentarySwitch {
  XycloidButtonNormal() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/button-normal-off.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/button-normal-on.svg")));
  }
};

struct XycloidKnobTiny : rack::RoundKnob {
  XycloidKnobTiny() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/knob-tiny.svg")));
    shadow->opacity = 0.f;
  }
};

struct XycloidKnobSmall : rack::RoundKnob {
  XycloidKnobSmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/knob-small.svg")));
    shadow->opacity = 0.f;
  }
};

struct XycloidKnobLarge : rack::RoundKnob {
  XycloidKnobLarge() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/knob-large.svg")));
    shadow->opacity = 0.f;
  }
};

struct XycloidSwitch2 : rack::SVGSwitch, rack::ToggleSwitch {
  XycloidSwitch2() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/switch-2-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/switch-2-high.svg")));
  }
};

struct XycloidSwitch3 : rack::SVGSwitch, rack::ToggleSwitch {
  XycloidSwitch3() {
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/switch-3-low.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/switch-3-mid.svg")));
    addFrame(rack::SVG::load(rack::assetPlugin(plugin, "res/xycloid/switch-3-high.svg")));
  }
};

struct XycloidPort : rack::SVGPort {
  XycloidPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/xycloid/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

XycloidWidget::XycloidWidget(rack::Module *module) : ModuleWidget(module, 11, "res/xycloid/panel.svg") {
  auto width = 11.f*5.08f;

  auto left_x = width/7.f;
  auto right_x = width - left_x;
  auto left_center_x = (right_x - left_x)/3.f + left_x;
  auto right_center_x = width - left_center_x;

  auto top_row_y = 30.f;
  auto row_spacing = 22.f;

  auto row = 0;

  install_input<XycloidPort>(XycloidModule::GEAR_RATIO_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobTiny>(XycloidModule::WOBBLE_RATIO_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobLarge>(XycloidModule::WOBBLE_RATIO_KNOB, {right_center_x, top_row_y + row*row_spacing});
  install_switch<XycloidSwitch2>(XycloidModule::QUANTIZE_WOBBLE_RATIO_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

  row++;
  install_input<XycloidPort>(XycloidModule::DEPTH_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobTiny>(XycloidModule::WOBBLE_DEPTH_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobLarge>(XycloidModule::WOBBLE_DEPTH_KNOB, {right_center_x, top_row_y + row*row_spacing});
  install_switch<XycloidSwitch3>(XycloidModule::WOBBLE_TYPE_SWITCH, {right_x, top_row_y + row*row_spacing}, 2, 2);

  row++;
  install_input<XycloidPort>(XycloidModule::SPEED_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobTiny>(XycloidModule::THROB_SPEED_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobLarge>(XycloidModule::THROB_SPEED_KNOB, {right_center_x, top_row_y + row*row_spacing}, 0.65f);
  install_knob<XycloidKnobSmall>(XycloidModule::WOBBLE_PHASE_KNOB, {right_x, top_row_y + row*row_spacing});

  top_row_y = 82.f;
  row_spacing = 15.f;
  row = 0;

  auto default_gain = XycloidModule::gain_range().normalize(1.f);
  row++;
  install_input<XycloidPort>(XycloidModule::X_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobSmall>(XycloidModule::X_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<XycloidSwitch2>(XycloidModule::X_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<XycloidPort>(XycloidModule::X_OUT, {right_x, top_row_y + row*row_spacing});

  row++;
  install_input<XycloidPort>(XycloidModule::Y_GAIN_CV, {left_x, top_row_y + row*row_spacing});
  install_knob<XycloidKnobSmall>(XycloidModule::Y_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
  install_switch<XycloidSwitch2>(XycloidModule::Y_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
  install_output<XycloidPort>(XycloidModule::Y_OUT, {right_x, top_row_y + row*row_spacing});
}
}
