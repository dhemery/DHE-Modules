#include "dhe-modules.h"
#include "module-widget.h"
#include "module.h"

#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class JusterFunction {
  const rack::Param &knob;
  Range const range;

public:
  JusterFunction(const rack::Param &knob, Range range)
      : knob{knob}, range{range} {}

  auto adjustment() const -> float { return range.scale(knob.value); }

  virtual auto apply(float input) const -> float = 0;
};

class JusterMultiplier : JusterFunction {
public:
  JusterMultiplier(const rack::Param &knob, const Range &range)
      : JusterFunction{knob, range} {}

  auto apply(float input) const -> float override {
    return input * adjustment();
  }
};

class JusterAdder : JusterFunction {
public:
  JusterAdder(const rack::Param &knob, const Range &range)
      : JusterFunction{knob, range} {}

  auto apply(float input) const -> float override {
    return input + adjustment();
  }
};

struct Juster : Module {
  class JusterChannel {
    const rack::Input &input;
    rack::Output &output;
    const rack::Param &mode;
    const JusterAdder offset;
    const JusterMultiplier av;
    const JusterMultiplier gain;

  public:
    JusterChannel(const rack::Input &input, rack::Output &output,
                  const rack::Param &mode, const rack::Param &knob)
        : input{input}, output{output}, mode{mode},
          offset{knob, Signal::bipolar_range}, av{knob, {-1.f, 1.f}},
          gain{knob, {0.f, 2.f}} {}

    auto adjust(float upstream) const -> float {
      auto selection = static_cast<int>(mode.value);
      auto in = input.active ? input.value : upstream;

      switch (selection) {
      case 0:
        output.value = offset.apply(in);
        break;
      case 1:
        output.value = av.apply(in);
        break;
      case 2:
        output.value = gain.apply(in);
        break;
      default:
        break;
      }
      return output.value;
    }
  };

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

  const std::vector<const JusterChannel> channels{
      {inputs[IN_1], outputs[OUT_1], params[MODE_1], params[KNOB_1]},
      {inputs[IN_2], outputs[OUT_2], params[MODE_2], params[KNOB_2]},
      {inputs[IN_3], outputs[OUT_3], params[MODE_3], params[KNOB_3]},
      {inputs[IN_4], outputs[OUT_4], params[MODE_4], params[KNOB_4]},
      {inputs[IN_5], outputs[OUT_5], params[MODE_5], params[KNOB_5]}};

  Juster() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto upstream = 0.f;
    for (const auto &channel : channels) {
      upstream = channel.adjust(upstream);
    }
  }
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
