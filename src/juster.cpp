#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"
#include "util/range.hpp"

namespace DHE {

struct Juster : Module {
  Juster() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto gain(float input, int knob) const -> float {
    static auto constexpr gain_range{Range{0.f, 2.f}};
    return input * gain_range.scale(params[knob].value);
  }

  auto av(float input, int knob) const -> float {
    static auto constexpr av_range{Range{-1.f, 1.f}};
    return input * av_range.scale(params[knob].value);
  }

  auto offset(float input, int knob) const -> float {
    static auto constexpr offset_range{Range{-5.f, 5.f}};
    return input + offset_range.scale(params[knob].value);
  }

  auto function_for_mode(int sw) const
      -> std::function<float(float, int)> const & {
    static auto functions = std::vector<std::function<float(float, int)>>{
        [this](float input, int knob) -> float { return offset(input, knob); },
        [this](float input, int knob) -> float { return av(input, knob); },
        [this](float input, int knob) -> float { return gain(input, knob); },
    };
    auto mode = static_cast<int>(params[sw].value);
    return functions[mode];
  }

  auto channel_input(int channel) const -> float {
    if (channel == 0 || inputs[IN_1 + channel].active) {
      return inputs[IN_1 + channel].value;
    } else {
      return outputs[OUT_1 + channel - 1].value;
    }
  }

  auto channel_value(int channel) const -> float {
    auto in = channel_input(channel);
    auto function = function_for_mode(MODE_1 + channel);
    return function(in, KNOB_1 + channel);
  }

  void step() override {
    for (auto chanel = 0; chanel < 5; chanel++) {
      outputs[OUT_1 + chanel].value = channel_value(chanel);
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
                     2);
      install_output(Juster::OUT_1 + row,
                     {right_x, top_row_y + row * row_spacing});
    }
  }
};

} // namespace DHE
rack::Model *modelJuster = rack::Model::create<DHE::Juster, DHE::JusterWidget>(
    "DHE-Modules", "Juster", "Juster", rack::UTILITY_TAG);
