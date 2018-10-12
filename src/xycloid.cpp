#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module.hpp"
#include "module-widget.hpp"

#include "util/controls.hpp"

namespace DHE {

struct Pole {
  const float two_pi = 2.f*std::acos(-1.f);
  float phase{0.f};
  float offset{0.f};

  void advance(float increment, float offset = 0.f) {
    this->offset = offset;
    phase += increment;
    phase -= std::trunc(phase); // Reduce phase to (-1, 1) to avoid eventual overflow
  }

  float x() const {
    return std::cos(two_pi*(phase+offset));
  }

  float y() const {
    return std::sin(two_pi*(phase+offset));
  }
};

struct Xycloid : Module {
  float wobble_ratio_offset = 0.f;
  Pole wobbler;
  Pole throbber;

  Xycloid() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  int wobble_type() const {
    return static_cast<int>(param(WOBBLE_TYPE_SWITCH));
  }

  bool quantize_wobble_ratio() const {
    return !param(QUANTIZE_WOBBLE_RATIO_SWITCH);
  }

  float wobble_ratio_rotation() const {
    return modulated(WOBBLE_RATIO_KNOB, GEAR_RATIO_CV, WOBBLE_RATIO_CV_ATTENUVERTER);
  }

  Range wobble_range() const {
    static constexpr auto inward_wobble_max = 16.f;
    static constexpr auto outward_wobble_max = -inward_wobble_max;
    static constexpr auto inward_wobble_range = Range{0.f, inward_wobble_max};
    static constexpr auto outward_wobble_range = Range{0.f, outward_wobble_max};
    static constexpr auto bidirectional_wobble_range = Range{inward_wobble_max, outward_wobble_max};
    static constexpr Range wobble_ratio_ranges[] = {
                                inward_wobble_range,
                                bidirectional_wobble_range,
                                outward_wobble_range
                            };
    return wobble_ratio_ranges[wobble_type()];
  }

  float wobble_ratio() const {
    auto wobble_ratio = wobble_range().scale(wobble_ratio_rotation()) + wobble_ratio_offset;
    return quantize_wobble_ratio() ? std::round(wobble_ratio) : wobble_ratio;
  }

  float wobble_depth() const {
    static constexpr auto wobble_depth_range = Range{0.f, 1.f};
    return wobble_depth_range.clamp(modulated(WOBBLE_DEPTH_KNOB, DEPTH_CV, WOBBLE_DEPTH_CV_ATTENUVERTER));
  }

  float throb_speed(int knob, int cv, int attenuator) const {
    auto rotation = modulated(knob, cv, attenuator);
    auto bipolar = BIPOLAR_PHASE_RANGE.scale(rotation);
    auto tapered = sigmoid(bipolar, 0.8f);
    return -10.f*tapered*rack::engineGetSampleTime();
  }

  float gain(int knob, int cv) const {
    return gain_range().scale(modulated(knob, cv));
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  void step() override {
    auto wobble_ratio = this->wobble_ratio();
    auto wobble_phase_offset = param(WOBBLE_PHASE_KNOB)-0.5f;
    if (wobble_ratio < 0.f) wobble_phase_offset*=-1.f;

    auto throb_speed = this->throb_speed(THROB_SPEED_KNOB, SPEED_CV, THROB_SPEED_CV_ATTENUVERTER);
    auto wobble_speed = wobble_ratio*throb_speed;
    auto wobble_depth = this->wobble_depth();
    auto throb_depth = 1.f - wobble_depth;

    throbber.advance(throb_speed);
    wobbler.advance(wobble_speed, wobble_phase_offset);
    auto x = throb_depth*throbber.x() + wobble_depth*wobbler.x();
    auto y = throb_depth*throbber.y() + wobble_depth*wobbler.y();

    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH);
    auto y_offset = param(Y_RANGE_SWITCH);
    outputs[X_OUT].value = 5.f*x_gain*(x + x_offset);
    outputs[Y_OUT].value = 5.f*y_gain*(y + y_offset);
  }

  enum ParameterIds {
    WOBBLE_RATIO_KNOB,
    WOBBLE_RATIO_CV_ATTENUVERTER,
    WOBBLE_TYPE_SWITCH,
    WOBBLE_DEPTH_KNOB,
    WOBBLE_DEPTH_CV_ATTENUVERTER,
    THROB_SPEED_KNOB,
    THROB_SPEED_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    QUANTIZE_WOBBLE_RATIO_SWITCH,
    WOBBLE_PHASE_KNOB,
    PARAMETER_COUNT
  };
  enum InputIds {
    GEAR_RATIO_CV,
    DEPTH_CV,
    SPEED_CV,
    X_GAIN_CV,
    Y_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds {
    X_OUT,
    Y_OUT,
    OUTPUT_COUNT
  };

  json_t *toJson() override {
      json_t *configuration = json_object();
      json_object_set_new(configuration, "musical_wobble_ratios", json_boolean(is_musical_wobble_ratios()));
      return configuration;
    }

    void fromJson(json_t *configuration) override {
        json_t *musical_wobble_ratios = json_object_get(configuration, "musical_wobble_ratios");
        set_wobble_ratios(json_is_true(musical_wobble_ratios));
    }

    void set_wobble_ratios(bool is_musical) {
        wobble_ratio_offset = is_musical ? 0.f : 1.f;
    }

    void toggle_musical_wobble_ratios() {
        set_wobble_ratios(!is_musical_wobble_ratios());
    }

    bool is_musical_wobble_ratios() {
        return wobble_ratio_offset == 0.f;
    }
  };

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

struct XycloidMusicalRatiosMenuItem : rack::MenuItem {
    Xycloid *xycloid;

    XycloidMusicalRatiosMenuItem(Xycloid *xycloid)
        : MenuItem{},
        xycloid{xycloid} {
        this->text = "Musical Ratios";
        this->rightText = "";
    }

	void onAction(rack::EventAction &e) override {
        xycloid->toggle_musical_wobble_ratios();
	}

	void step() override {
		rightText = xycloid->is_musical_wobble_ratios() ? "✔" : "";
		rack::MenuItem::step();
	}
};

struct XycloidWidget : public ModuleWidget {
  XycloidWidget(rack::Module *module) : ModuleWidget(module, 11, "res/xycloid/panel.svg") {
    auto width = 11.f*5.08f;

    auto left_x = width/7.f;
    auto right_x = width - left_x;
    auto left_center_x = (right_x - left_x)/3.f + left_x;
    auto right_center_x = width - left_center_x;

    auto top_row_y = 30.f;
    auto row_spacing = 22.f;

    auto row = 0;

    install_input<XycloidPort>(Xycloid::GEAR_RATIO_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobTiny>(Xycloid::WOBBLE_RATIO_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobLarge>(Xycloid::WOBBLE_RATIO_KNOB, {right_center_x, top_row_y + row*row_spacing});
    install_switch<XycloidSwitch2>(Xycloid::QUANTIZE_WOBBLE_RATIO_SWITCH, {right_x, top_row_y + row*row_spacing}, 1, 1);

    row++;
    install_input<XycloidPort>(Xycloid::DEPTH_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobTiny>(Xycloid::WOBBLE_DEPTH_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobLarge>(Xycloid::WOBBLE_DEPTH_KNOB, {right_center_x, top_row_y + row*row_spacing});
    install_switch<XycloidSwitch3>(Xycloid::WOBBLE_TYPE_SWITCH, {right_x, top_row_y + row*row_spacing}, 2, 2);

    row++;
    install_input<XycloidPort>(Xycloid::SPEED_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobTiny>(Xycloid::THROB_SPEED_CV_ATTENUVERTER, {left_center_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobLarge>(Xycloid::THROB_SPEED_KNOB, {right_center_x, top_row_y + row*row_spacing}, 0.65f);
    install_knob<XycloidKnobSmall>(Xycloid::WOBBLE_PHASE_KNOB, {right_x, top_row_y + row*row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;
    row = 0;

    auto default_gain = Xycloid::gain_range().normalize(1.f);
    row++;
    install_input<XycloidPort>(Xycloid::X_GAIN_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobSmall>(Xycloid::X_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
    install_switch<XycloidSwitch2>(Xycloid::X_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
    install_output<XycloidPort>(Xycloid::X_OUT, {right_x, top_row_y + row*row_spacing});

    row++;
    install_input<XycloidPort>(Xycloid::Y_GAIN_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<XycloidKnobSmall>(Xycloid::Y_GAIN_KNOB, {left_center_x, top_row_y + row*row_spacing}, default_gain);
    install_switch<XycloidSwitch2>(Xycloid::Y_RANGE_SWITCH, {right_center_x, top_row_y + row*row_spacing}, 1, 0);
    install_output<XycloidPort>(Xycloid::Y_OUT, {right_x, top_row_y + row*row_spacing});
  }

  void appendContextMenu(rack::Menu *menu) override {
      Xycloid *xycloid = dynamic_cast<Xycloid*>(module);
      menu->addChild(rack::construct<rack::MenuLabel>());
      menu->addChild(rack::construct<rack::MenuLabel>(&rack::MenuLabel::text, "Options"));
      menu->addChild(new XycloidMusicalRatiosMenuItem(xycloid));
  }
};

}
rack::Model *modelXycloid = rack::Model::create<DHE::Xycloid, DHE::XycloidWidget>("DHE-Modules", "Xycloid", "Xycloid", rack::FUNCTION_GENERATOR_TAG, rack::LFO_TAG);