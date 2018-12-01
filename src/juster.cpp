#include "dhe-modules.h"
#include "module-widget.h"

#include "util/knob.h"
#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class JusterChannel {
public:
  JusterChannel(rack::Module *module, int input, int function_switch_param,
                int amount_knob_param, int output)
      : input_port{module->inputs[input]},
        function_selector{module->params[function_switch_param].value},
        amount{module->params[amount_knob_param].value},
        output{module->outputs[output].value} {}

  auto adjust(float upstream) -> float {
    auto input = input_port.active ? input_port.value : upstream;
    auto function_selection = static_cast<int>(function_selector);
    switch (function_selection) {
    case OFFSET:
      output = input + Signal::bipolar_range.scale(amount);
      break;
    case AV:
      output = input * Knob::av_multiplier(amount);
      break;
    case GAIN:
      output = input * Knob::gain_multiplier(amount);
      break;
    default:
      output = -9.f;
      break;
    }
    return output;
  }

private:
  enum ChannelFunction { OFFSET, AV, GAIN };

  const rack::Input &input_port;
  const float &function_selector;
  const float &amount;
  float &output;
};

class Juster : public rack::Module {
public:
  Juster() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

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
    MODE_1,
    MODE_2,
    MODE_3,
    MODE_4,
    MODE_5,
    PARAMETER_COUNT
  };

  enum InputIds { IN_1, IN_2, IN_3, IN_4, IN_5, INPUT_COUNT };

  enum OutputIds { OUT_1, OUT_2, OUT_3, OUT_4, OUT_5, OUTPUT_COUNT };

private:
  std::vector<JusterChannel> channels{
      {this, IN_1, MODE_1, KNOB_1, OUT_1}, {this, IN_2, MODE_2, KNOB_2, OUT_2},
      {this, IN_3, MODE_3, KNOB_3, OUT_3}, {this, IN_4, MODE_4, KNOB_4, OUT_4},
      {this, IN_5, MODE_5, KNOB_5, OUT_5},
  };
};

struct JusterWidget : public ModuleWidget {
  explicit JusterWidget(rack::Module *module)
      : ModuleWidget(module, 11, "juster") {
    auto widget_right_edge = width();

    auto left_x = widget_right_edge / 7.f;
    auto right_x = widget_right_edge - left_x;
    auto left_center_x = (right_x - left_x) / 3.f + left_x;
    auto right_center_x = widget_right_edge - left_center_x;

    auto top_row_y = 27.f;
    auto row_spacing = 20.f;

    for (auto row = 0; row < 5; row++) {
      install_input(Juster::IN_1 + row,
                    {left_x, top_row_y + row * row_spacing});
      install_knob(
          "large", Juster::KNOB_1 + row,
          {left_center_x + 1.25f, top_row_y - 1.25f + row * row_spacing});
      install_switch(Juster::MODE_1 + row,
                     {right_center_x, top_row_y - 1.25f + row * row_spacing}, 2,
                     0);
      install_output(Juster::OUT_1 + row,
                     {right_x, top_row_y + row * row_spacing});
    }
  }
};

} // namespace DHE
rack::Model *modelJuster = rack::Model::create<DHE::Juster, DHE::JusterWidget>(
    "DHE-Modules", "Juster", "Juster", rack::UTILITY_TAG);
