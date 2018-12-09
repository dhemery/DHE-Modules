#include "dhe-modules.h"
#include "module-widget.h"

#include "util/knob.h"
#include "util/signal.h"

namespace DHE {

class FuncChannel {
public:
  FuncChannel(rack::Module *module,
              int input,
              int operator_switch_param,
              int addition_range_switch_param,
              int multiplication_range_switch_param,
              int amount_knob_param,
              int output) : input_port{module->inputs[input]},
                            operator_selector{module->params[operator_switch_param].value},
                            addition_range_selector{module->params[addition_range_switch_param].value},
                            multiplication_range_selector{module->params[multiplication_range_switch_param].value},
                            amount{module->params[amount_knob_param].value},
                            output{module->outputs[output].value} {}

  auto adjust(float upstream) -> float {
    static const std::vector<Range> multiplication_ranges{{0.f, 1.f}, {-1.f, 1.f}, {0.f, 2.f}, {-2.f, 2.f}};
    static const std::vector<Range> addition_ranges{{0.f, 5.f}, {-5.f, 5.f}, {0.f, 10.f}, {-10.f, 10.f}};
    auto input = input_port.active ? input_port.value : upstream;
    if (operator_selector > 0.5f) {
      auto selection = static_cast<int>(multiplication_range_selector);
      auto range = multiplication_ranges[selection];
      output = input*range.scale(amount);
    } else {
      auto selection = static_cast<int>(addition_range_selector);
      auto range = addition_ranges[selection];
      output = input + range.scale(amount);
    }
    return output;
  }

private:
  const rack::Input &input_port;
  const float &operator_selector;
  const float &addition_range_selector;
  const float &multiplication_range_selector;
  const float &amount;
  float &output;
};

class Func : public rack::Module {
public:
  Func() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override { channel.adjust(0.f); }

  enum ParameterIds { KNOB, OPERATOR_SWITCH, ADDITION_RANGE_SWITCH, MULTIPLICATION_RANGE_SWITCH, PARAMETER_COUNT };

  enum InputIds { IN, INPUT_COUNT };

  enum OutputIds { OUT, OUTPUT_COUNT };

private:
  FuncChannel channel{this, IN, OPERATOR_SWITCH, ADDITION_RANGE_SWITCH, MULTIPLICATION_RANGE_SWITCH, KNOB, OUT};
};

class Func6 : public rack::Module {
  static constexpr auto channel_count = 6;

public:
  Func6() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    for (int i = 0; i < channel_count; i++) {
      channels.emplace_back(this, IN + i, OPERATOR_SWITCH + i, ADDITION_RANGE_SWITCH + i, MULTIPLICATION_RANGE_SWITCH + i, KNOB + i, OUT + i);
    }
  }

  void step() override {
    auto upstream = 0.f;
    for (auto &channel : channels) {
      upstream = channel.adjust(upstream);
    }
  }

  enum ParameterIds {
    KNOB,
    OPERATOR_SWITCH = KNOB + channel_count,
    ADDITION_RANGE_SWITCH = OPERATOR_SWITCH + channel_count,
    MULTIPLICATION_RANGE_SWITCH = ADDITION_RANGE_SWITCH + channel_count,
    PARAMETER_COUNT = MULTIPLICATION_RANGE_SWITCH + channel_count
  };

  enum InputIds { IN, INPUT_COUNT = IN + channel_count };

  enum OutputIds { OUT, OUTPUT_COUNT = OUT + channel_count };

private:
  std::vector<FuncChannel> channels{};
};

template<typename TDisplay>
class OperatorSwitch : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  OperatorSwitch() {
    addFrame(TDisplay::svg("switch-2-low"));
    addFrame(TDisplay::svg("switch-2-high"));
  }

  void onChange(rack::EventChange &e) override {
    rack::SVGSwitch::onChange(e);
    set_range_switch_visibility();
  }

  void set_range_switch_visibility() {
    if(multiplication_range_switch == nullptr) return;
    if(value > 0.5) {
      multiplication_range_switch->visible = true;
      addition_range_switch->visible = false;
    } else {
      multiplication_range_switch->visible = false;
      addition_range_switch->visible = true;
    }
  }

  void set_range_switches(SwitchWidget *addition_range_switch, SwitchWidget *multiplication_range_switch) {
    this->addition_range_switch = addition_range_switch;
    this->multiplication_range_switch = multiplication_range_switch;
    set_range_switch_visibility();
  }

  static auto create(rack::Module *module, const std::string &name, int index, rack::Vec center, SwitchWidget *addition_range_switch, SwitchWidget *multiplication_range_switch) -> OperatorSwitch<TDisplay> * {
    auto switch_widget = rack::ParamWidget::create<OperatorSwitch<TDisplay>>({0, 0}, module, index, 0.f, 1.f, 0.f);
    moveTo(switch_widget->box, rack::mm2px(center));
    switch_widget->set_range_switches(addition_range_switch, multiplication_range_switch);
    return switch_widget;
  }
private:
  SwitchWidget *addition_range_switch = nullptr;
  SwitchWidget *multiplication_range_switch = nullptr;
};

struct FuncWidget : public ModuleWidget<FuncWidget, Func> {
  static constexpr auto resource_name = "func";

  explicit FuncWidget(Func *module)
      : ModuleWidget(module, 3) {


    auto widget_right_edge = width();

    auto x = widget_right_edge/2.f;

    auto top = 23.f;
    auto bottom = 108.f;
    auto row_count = 6;
    auto row_spacing = (bottom - top)/(row_count - 1);
    auto port_offset = 1.25f;

    using Position = rack::Vec;
    auto in_port_center = Position{x, top+port_offset};
    auto operator_switch_center = Position{x, top + row_spacing*1};
    auto knob_center = Position{x, top + row_spacing*2};
    auto range_switch_center = Position{x, top + row_spacing*3};
    auto out_port_center = Position{x, top + row_spacing*5 + port_offset};

    install_input(Func::IN, in_port_center);
    install_knob("large", Func::KNOB, knob_center);
    install_output(Func::OUT, out_port_center);

    auto multiplication_range_switch = create_toggle("mult", Func::MULTIPLICATION_RANGE_SWITCH, range_switch_center, 3, 2);
    addParam(multiplication_range_switch);

    auto addition_range_switch = create_toggle("add", Func::ADDITION_RANGE_SWITCH, range_switch_center, 3, 1);
    addParam(addition_range_switch);

    auto operator_switch = OperatorSwitch<FuncWidget>::create(module, resource_name, Func::OPERATOR_SWITCH, operator_switch_center, addition_range_switch, multiplication_range_switch);
    addParam(operator_switch);
  }
};

struct Func6Widget : public ModuleWidget<Func6Widget, Func6> {
  static constexpr auto resource_name = "func6";

  explicit Func6Widget(Func6 *module)
      : ModuleWidget(module, 12) {
    auto widget_right_edge = width();

    auto knob_x = widget_right_edge/2.f;
    auto in_port_x = widget_right_edge/7.f;
    auto out_port_x = widget_right_edge - in_port_x;
    auto operator_switch_x = (knob_x - in_port_x)/2.f + in_port_x;
    auto range_switch_x = widget_right_edge - operator_switch_x;

    auto top = 23.f;
    auto bottom = 108.f;
    auto row_count = 6;
    auto row_spacing = (bottom - top)/(row_count - 1);
    auto port_offset = 1.25f;

    using Position = rack::Vec;

    for (auto row = 0; row < row_count; row++) {
      auto y = top + row*row_spacing;
      auto port_y = y + port_offset;

      auto in_port_center = Position{in_port_x, port_y};
      auto operator_switch_center = Position{operator_switch_x, y};
      auto knob_center = Position{knob_x, y};
      auto range_switch_center = Position{range_switch_x, y};
      auto out_port_center = Position{out_port_x, port_y};

      install_input(Func6::IN + row, in_port_center);
      install_knob("large", Func6::KNOB + row, knob_center);
      install_output(Func6::OUT + row, out_port_center);

      auto multiplication_range_switch = create_toggle("mult", Func6::MULTIPLICATION_RANGE_SWITCH + row, range_switch_center, 3, 2);
      addParam(multiplication_range_switch);

      auto addition_range_switch = create_toggle("add", Func6::ADDITION_RANGE_SWITCH + row, range_switch_center, 3, 1);
      addParam(addition_range_switch);

      auto operator_switch = OperatorSwitch<Func6Widget>::create(module, resource_name, Func6::OPERATOR_SWITCH + row, operator_switch_center, addition_range_switch, multiplication_range_switch);
      addParam(operator_switch);
    }
  }
};

} // namespace DHE
rack::Model *modelFunc = rack::Model::create<DHE::Func, DHE::FuncWidget>(
    "DHE-Modules", "Func", "Func", rack::UTILITY_TAG);
rack::Model *modelFunc6 = rack::Model::create<DHE::Func6, DHE::Func6Widget>(
    "DHE-Modules", "Func 6", "Func 6", rack::UTILITY_TAG);
