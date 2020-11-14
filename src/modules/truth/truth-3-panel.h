#include "truth-control-ids.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
namespace truth {

class Truth3Panel : public rack::app::ModuleWidget {
public:
  explicit Truth3Panel(rack::engine::Module *module) {
    auto constexpr svg_dir = "truth";

    setModule(module);
    setPanel(load_svg(svg_dir, "truth-3"));
    install_screws(this, hp);

    for (int i = 0; i < input_count; i++) {
      auto const input_y = port_top + port_dy * static_cast<float>(i);
      addInput(Jack::input(svg_dir, module, input_port_x, input_y,
                           Input::Input + i));
      addParam(Button::momentary(svg_dir, module, input_button_x, input_y,
                                 Param::InputOverride + i));
    }

    for (int i = 0; i < input_count; i++) {
      auto const condition_x =
          condition_left + condition_dx * static_cast<float>(i);
      addParam(Toggle::stepper(svg_dir, "condition", 5, module, condition_x,
                               condition_y, Param::Condition + i));
    }

    auto constexpr outcome_count = 1 << input_count;
    for (int i = 0; i < outcome_count; i++) {
      auto const outcome_y = outcome_top + outcome_dy * static_cast<float>(i);
      addParam(Toggle::stepper(svg_dir, "outcome", 2, module, outcome_x,
                               outcome_y, Param::Outcome + i));
    }

    addOutput(
        Jack::output(svg_dir, module, output_x, port_top + port_dy, Output::Q));
    addOutput(Jack::output(svg_dir, module, output_x, port_top + port_dy * 2.F,
                           Output::NotQ));
  }

  static auto constexpr input_count = 3;
  using Param = ParamIds<input_count>;
  using Input = InputIds<input_count>;
  using Output = OutputIds;

  static auto constexpr hp = 10;
  static auto constexpr port_top = 76.2F;
  static auto constexpr port_dy = 15.22F;
  static auto constexpr input_port_x = 10.16F;
  static auto constexpr input_button_x = 18.36F;
  static auto constexpr output_x = 40.64F;
  static auto constexpr condition_left = 9.939F;
  static auto constexpr condition_dx = 10.307F;
  static auto constexpr condition_y = 22.931F;
  static auto constexpr outcome_x = 40.861F;
  static auto constexpr outcome_top = 28.24F;
  static auto constexpr outcome_dy = 4.826F;
};
} // namespace truth
} // namespace dhe
