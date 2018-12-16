#include "dhe-modules.h"
#include "display/controls.h"
#include "display/panel.h"
#include "util/rotation.h"
#include "util/signal.h"

namespace DHE {

class FuncChannel {
public:
  FuncChannel(rack::Module *module, int input, int operator_switch_param,
              int addition_range_switch_param,
              int multiplication_range_switch_param, int amount_knob_param,
              int output)
      : input_port{module->inputs[input]},
        operator_selector{module->params[operator_switch_param].value},
        addition_range_selector{
            module->params[addition_range_switch_param].value},
        multiplication_range_selector{
            module->params[multiplication_range_switch_param].value},
        amount{module->params[amount_knob_param].value},
        output{module->outputs[output].value} {}

  auto adjust(float upstream) -> float {
    static const std::vector<Range> multiplication_ranges{
        {0.f, 1.f}, {-1.f, 1.f}, {0.f, 2.f}, {-2.f, 2.f}};
    static const std::vector<Range> addition_ranges{
        {0.f, 5.f}, {-5.f, 5.f}, {0.f, 10.f}, {-10.f, 10.f}};
    auto input = input_port.active ? input_port.value : upstream;
    if (operator_selector > 0.5f) {
      auto selection = static_cast<int>(multiplication_range_selector);
      auto range = multiplication_ranges[selection];
      output = input * range.scale(amount);
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

  enum ParameterIds {
    KNOB,
    OPERATOR_SWITCH,
    ADDITION_RANGE_SWITCH,
    MULTIPLICATION_RANGE_SWITCH,
    PARAMETER_COUNT
  };

  enum InputIds { IN, INPUT_COUNT };

  enum OutputIds { OUT, OUTPUT_COUNT };

private:
  FuncChannel channel{this,
                      IN,
                      OPERATOR_SWITCH,
                      ADDITION_RANGE_SWITCH,
                      MULTIPLICATION_RANGE_SWITCH,
                      KNOB,
                      OUT};
};

class Func6 : public rack::Module {
  static constexpr auto channel_count = 6;

public:
  Func6() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    for (int i = 0; i < channel_count; i++) {
      channels.emplace_back(this, IN + i, OPERATOR_SWITCH + i,
                            ADDITION_RANGE_SWITCH + i,
                            MULTIPLICATION_RANGE_SWITCH + i, KNOB + i, OUT + i);
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

template <typename P> class MultiplicationRangeSwitch : public Toggle<P, 4> {
public:
  MultiplicationRangeSwitch() : Toggle<P, 4>{"button-mult"} {}

  void enable() { this->visible = true; }

  void disable() { this->visible = false; }
};

template <typename P> class AdditionRangeSwitch : public Toggle<P, 4> {
public:
  AdditionRangeSwitch() : Toggle<P, 4>{"button-add"} {}
  void enable() { this->visible = true; }
  void disable() { this->visible = false; }
};

template <typename P> class OperatorSwitch : public Toggle<P, 2> {
public:
  void onChange(rack::EventChange &e) override {
    rack::SVGSwitch::onChange(e);
    set_range_switch_visibility();
  }

  void set_range_switch_visibility() {
    if (multiplication_range_switch == nullptr) {
      return;
    }

    if (this->value > 0.5) {
      multiplication_range_switch->enable();
      addition_range_switch->disable();
    } else {
      multiplication_range_switch->disable();
      addition_range_switch->enable();
    }
  }

  void set_range_switches(
      AdditionRangeSwitch<P> *addition_range_switch,
      MultiplicationRangeSwitch<P> *multiplication_range_switch) {
    this->addition_range_switch = addition_range_switch;
    this->multiplication_range_switch = multiplication_range_switch;
    set_range_switch_visibility();
  }

private:
  AdditionRangeSwitch<P> *addition_range_switch = nullptr;
  MultiplicationRangeSwitch<P> *multiplication_range_switch = nullptr;
};

class FuncPanel : public Panel<FuncPanel> {
public:
  explicit FuncPanel(Func *module) : Panel{module, hp} {

    auto widget_right_edge = width();

    auto x = widget_right_edge / 2.f;

    auto top = 23.f;
    auto bottom = 108.f;
    auto row_count = 6;
    auto row_spacing = (bottom - top) / (row_count - 1);
    auto port_offset = 1.25f;

    auto row_1 = top + port_offset;
    auto row_2 = top + row_spacing;
    auto row_3 = top + row_spacing * 2;
    auto row_4 = top + row_spacing * 3;
    auto row_6 = top + row_spacing * 5 + port_offset;

    install(x, row_1, input(Func::IN));
    install(x, row_3, knob<LargeKnob>(Func::KNOB));
    install(x, row_6, output(Func::OUT));

    auto multiplication_range_switch =
        toggle<MultiplicationRangeSwitch>(Func::MULTIPLICATION_RANGE_SWITCH, 2);
    install(x, row_4, multiplication_range_switch);

    auto addition_range_switch =
        toggle<AdditionRangeSwitch>(Func::ADDITION_RANGE_SWITCH, 1);
    install(x, row_4, addition_range_switch);

    auto operator_switch = toggle<OperatorSwitch>(Func::OPERATOR_SWITCH, 0);
    operator_switch->set_range_switches(addition_range_switch,
                                        multiplication_range_switch);

    install(x, row_2, operator_switch);
  }

  static constexpr auto module_svg_dir = "func";

private:
  static constexpr auto hp = 3;
};

class Func6Panel : public Panel<Func6Panel> {
public:
  explicit Func6Panel(Func6 *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_3 = widget_right_edge / 2.f;
    auto column_1 = widget_right_edge / 7.f;
    auto column_5 = widget_right_edge - column_1;
    auto column_2 = (column_3 - column_1) / 2.f + column_1;
    auto column_4 = widget_right_edge - column_2;

    auto top = 23.f;
    auto bottom = 108.f;
    auto row_count = 6;
    auto row_spacing = (bottom - top) / (row_count - 1);
    auto port_offset = 1.25f;

    for (auto row = 0; row < row_count; row++) {
      auto y = top + row * row_spacing;
      auto port_y = y + port_offset;

      install(column_1, port_y, input(Func6::IN + row));
      install(column_3, y, knob<LargeKnob>(Func6::KNOB + row));
      install(column_5, port_y, output(Func6::OUT + row));

      auto multiplication_range_switch = toggle<MultiplicationRangeSwitch>(
          Func6::MULTIPLICATION_RANGE_SWITCH + row, 2);
      install(column_4, y, multiplication_range_switch);

      auto addition_range_switch =
          toggle<AdditionRangeSwitch>(Func6::ADDITION_RANGE_SWITCH + row, 1);
      install(column_4, y, addition_range_switch);

      auto operator_switch =
          toggle<OperatorSwitch>(Func6::OPERATOR_SWITCH + row, 0);
      operator_switch->set_range_switches(addition_range_switch,
                                          multiplication_range_switch);
      install(column_2, y, operator_switch);
    }
  }

  static constexpr auto module_svg_dir = "func6";

private:
  static constexpr auto hp = 12;
};

} // namespace DHE
rack::Model *modelFunc = rack::Model::create<DHE::Func, DHE::FuncPanel>(
    "DHE-Modules", "Func", "Func", rack::UTILITY_TAG);
rack::Model *modelFunc6 = rack::Model::create<DHE::Func6, DHE::Func6Panel>(
    "DHE-Modules", "Func6", "Func 6", rack::UTILITY_TAG);
