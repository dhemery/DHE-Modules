#include "dhe-modules.h"
#include "module-widget.h"

#include "util/knob.h"
#include "util/signal.h"

namespace DHE {

class FuncChannel {
public:
  FuncChannel(rack::Module *module,
               int input,
               int function_switch_param,
               int range_switch_param,
               int amount_knob_param,
               int output) : input_port{module->inputs[input]},
                             function_selector{module->params[function_switch_param].value},
                             range_selector{module->params[range_switch_param].value},
                             amount{module->params[amount_knob_param].value},
                             output{module->outputs[output].value} {}

  auto adjust(float upstream) -> float {
    static const std::vector<Range> multiplication_ranges{{0.f,1.f}, {-1.f,1.f}, {0.f,2.f}, {-2.f,2.f}};
    static const std::vector<Range> addition_ranges{{0.f,5.f}, {-5.f,5.f}, {0.f,10.f}, {-10.f,10.f}};
    auto input = input_port.active ? input_port.value : upstream;
    auto range_selection = static_cast<int>(range_selector);
    if (function_selector > 0.5f) {
      auto range = multiplication_ranges[range_selection];
      output = input*range.scale(amount);
    } else {
      auto range = addition_ranges[range_selection];
      output = input + range.scale(amount);
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

class Func : public rack::Module {
public:
  Func() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override { channel.adjust(0.f); }

  enum ParameterIds { KNOB, OPERATOR_SWITCH, RANGE_SWITCH, PARAMETER_COUNT };

  enum InputIds { IN, INPUT_COUNT };

  enum OutputIds { OUT, OUTPUT_COUNT };

private:
  FuncChannel channel{this, IN, OPERATOR_SWITCH, RANGE_SWITCH, KNOB, OUT};
};

class Func6 : public rack::Module {
public:
  Func6() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

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
  std::vector<FuncChannel> channels{
      {this, IN_1, OPERATOR_SWITCH_1, RANGE_SWITCH_1, KNOB_1, OUT_1},
      {this, IN_2, OPERATOR_SWITCH_2, RANGE_SWITCH_2, KNOB_2, OUT_2},
      {this, IN_3, OPERATOR_SWITCH_3, RANGE_SWITCH_3, KNOB_3, OUT_3},
      {this, IN_4, OPERATOR_SWITCH_4, RANGE_SWITCH_4, KNOB_4, OUT_4},
      {this, IN_5, OPERATOR_SWITCH_5, RANGE_SWITCH_5, KNOB_5, OUT_5},
      {this, IN_6, OPERATOR_SWITCH_6, RANGE_SWITCH_6, KNOB_6, OUT_6}
  };
};

struct FuncWidget : public ModuleWidget {
  explicit FuncWidget(rack::Module *module)
      : ModuleWidget(module, 3, "func") {
    auto widget_right_edge = width();

    auto center_x = widget_right_edge/2.f;

    auto top = 23.f;
    auto bottom = 108.f;

    auto row_count = 6;
    auto row_spacing = (bottom - top)/(row_count - 1);

    auto port_offset = 1.25f;

    auto y = top;
    install_input(Func::IN, {center_x, y + port_offset});
    y += row_spacing;
    install_switch(Func::OPERATOR_SWITCH, {center_x, y}, 1, 1);
    y += row_spacing;
    install_knob("large", Func::KNOB, {center_x, y});
    y += row_spacing;
    install_toggle("range", Func::RANGE_SWITCH, {center_x, y}, 3, 0);
    y += row_spacing;
    y += row_spacing;
    install_output(Func::OUT, {center_x, y + port_offset});
  }
};

struct FuncsWidget : public ModuleWidget {
  explicit FuncsWidget(rack::Module *module)
      : ModuleWidget(module, 12, "func6") {
    auto widget_right_edge = width();

    auto center_x = widget_right_edge/2.f;
    auto left_x = widget_right_edge/7.f;
    auto right_x = widget_right_edge - left_x;
    auto left_center_x = (center_x - left_x)/2.f + left_x;
    auto right_center_x = widget_right_edge - left_center_x;

    auto top = 23.f;
    auto bottom = 108.f;

    auto row_count = 6;
    auto row_spacing = (bottom - top)/(row_count - 1);

    auto port_offset = 1.25f;

    for (auto row = 0; row < row_count; row++) {
      auto y = top + row*row_spacing;

      install_input(Func6::IN_1 + row, {left_x, y + port_offset});
      install_switch(Func6::OPERATOR_SWITCH_1 + row, {left_center_x, y}, 1, 1);
      install_knob("large", Func6::KNOB_1 + row, {center_x, y});
      install_toggle("range", Func6::RANGE_SWITCH_1 + row, {right_center_x, y}, 3, 0);
      install_output(Func6::OUT_1 + row, {right_x, y + port_offset});
    }
  }
};

} // namespace DHE
rack::Model *modelFunc = rack::Model::create<DHE::Func, DHE::FuncWidget>(
    "DHE-Modules", "Func", "Func", rack::UTILITY_TAG);
rack::Model *modelFunc6 = rack::Model::create<DHE::Func6, DHE::FuncsWidget>(
    "DHE-Modules", "Func 6", "Func 6", rack::UTILITY_TAG);
