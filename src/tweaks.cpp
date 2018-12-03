#include "dhe-modules.h"
#include "module-widget.h"

#include "util/knob.h"
#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class TweakChannel {
public:
  TweakChannel(rack::Module *module, int input, int function_switch_param,
               int range_switch_param, int amount_knob_param, int output)
      : input_port{module->inputs[input]},
        function_selector{module->params[function_switch_param].value},
        range_selector{module->params[range_switch_param].value},
        amount{module->params[amount_knob_param].value},
        output{module->outputs[output].value} {}

  auto adjust(float upstream) -> float {
    auto input = input_port.active ? input_port.value : upstream;
    if (function_selector > 0.5f) {
      if (range_selector > 0.5f) {
        output = input * Knob::gain_range.scale(amount);
      } else {
        output = input * Knob::av_range.scale(amount);
      }
    } else {
      if (range_selector > 0.5f) {
        output = input + Signal::unipolar_range.scale(amount);
      } else {
        output = input + Signal::bipolar_range.scale(amount);
      }
    }
    return output;
  }

private:
  const rack::Input &input_port;
  const float &function_selector;
  const float &range_selector;
  const float &amount;
  float &output;
};

class Tweak : public rack::Module {
public:
  Tweak() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override { channel.adjust(0.f); }

  enum ParameterIds { KNOB, OPERATOR_SWITCH, RANGE_SWITCH, PARAMETER_COUNT };

  enum InputIds { IN, INPUT_COUNT };

  enum OutputIds { OUT, OUTPUT_COUNT };

private:
  TweakChannel channel{this, IN, OPERATOR_SWITCH, RANGE_SWITCH, KNOB, OUT};
};

class Tweaks : public rack::Module {
public:
  Tweaks() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto upstream = 0.f;
    for (auto &channel : channels) {
      upstream = channel.adjust(upstream);
    }
  }

  enum ParameterIds {
    KNOB_1,
    KNOB_2,
    KNOB_3,
    KNOB_4,
    KNOB_5,
    KNOB_6,
    OPERATOR_SWITCH_1,
    OPERATOR_SWITCH_2,
    OPERATOR_SWITCH_3,
    OPERATOR_SWITCH_4,
    OPERATOR_SWITCH_5,
    OPERATOR_SWITCH_6,
    RANGE_SWITCH_1,
    RANGE_SWITCH_2,
    RANGE_SWITCH_3,
    RANGE_SWITCH_4,
    RANGE_SWITCH_5,
    RANGE_SWITCH_6,
    PARAMETER_COUNT
  };

  enum InputIds { IN_1, IN_2, IN_3, IN_4, IN_5, IN_6, INPUT_COUNT };

  enum OutputIds { OUT_1, OUT_2, OUT_3, OUT_4, OUT_5, OUT_6, OUTPUT_COUNT };

private:
  std::vector<TweakChannel> channels{
      {this, IN_1, OPERATOR_SWITCH_1, RANGE_SWITCH_1, KNOB_1, OUT_1},
      {this, IN_2, OPERATOR_SWITCH_2, RANGE_SWITCH_2, KNOB_2, OUT_2},
      {this, IN_3, OPERATOR_SWITCH_3, RANGE_SWITCH_3, KNOB_3, OUT_3},
      {this, IN_4, OPERATOR_SWITCH_4, RANGE_SWITCH_4, KNOB_4, OUT_4},
      {this, IN_5, OPERATOR_SWITCH_5, RANGE_SWITCH_5, KNOB_5, OUT_5},
      {this, IN_6, OPERATOR_SWITCH_6, RANGE_SWITCH_6, KNOB_6, OUT_6},
  };
};

struct TweakWidget : public ModuleWidget {
  explicit TweakWidget(rack::Module *module)
      : ModuleWidget(module, 3, "tweak") {
    auto widget_right_edge = width();

    auto center_x = widget_right_edge / 2.f;

    auto top = 23.f;
    auto bottom = 108.f;
    auto center_y = (bottom - top) / 2.f + top;

    auto row_count = 6;
    auto row_spacing = (bottom - top) / (row_count -1);

    auto port_offset = 1.25f;

    auto y = top;
    install_switch(Tweak::OPERATOR_SWITCH, {center_x, y}, 1, 1);
    y+= row_spacing;
    install_switch(Tweak::RANGE_SWITCH, {center_x, y}, 1, 1);
    y+= row_spacing;
    install_knob("large", Tweak::KNOB, {center_x, center_y});
    y+= row_spacing;
    y+= row_spacing;
    install_input(Tweak::IN, {center_x, y+port_offset});
    y+= row_spacing;
    install_output(Tweak::OUT, {center_x, y + port_offset});
  }
};

struct TweaksWidget : public ModuleWidget {
  explicit TweaksWidget(rack::Module *module)
      : ModuleWidget(module, 12, "tweaks") {
    auto widget_right_edge = width();

    auto center_x = widget_right_edge / 2.f;
    auto left_x = widget_right_edge / 7.f;
    auto right_x = widget_right_edge - left_x;
    auto left_center_x = (center_x - left_x) / 2.f + left_x;
    auto right_center_x = widget_right_edge - left_center_x;

    auto top = 23.f;
    auto bottom = 108.f;

    auto row_count = 6;
    auto row_spacing = (bottom - top) / (row_count -1);

    auto port_offset = 1.25f;

    for (auto row = 0; row < row_count; row++) {
      auto y = top + row * row_spacing;

      install_input(Tweaks::IN_1 + row, {left_x, y+port_offset});
      install_switch(Tweaks::OPERATOR_SWITCH_1 + row, {left_center_x, y}, 1, 1);
      install_knob("large", Tweaks::KNOB_1 + row, {center_x, y});
      install_switch(Tweaks::RANGE_SWITCH_1 + row, {right_center_x, y}, 1, 1);
      install_output(Tweaks::OUT_1 + row, {right_x, y+port_offset});
    }
  }
};

} // namespace DHE
rack::Model *modelTweak = rack::Model::create<DHE::Tweak, DHE::TweakWidget>(
    "DHE-Modules", "Tweak", "Tweak", rack::UTILITY_TAG);
rack::Model *modelTweaks = rack::Model::create<DHE::Tweaks, DHE::TweaksWidget>(
    "DHE-Modules", "Tweaks", "Tweaks", rack::UTILITY_TAG);
