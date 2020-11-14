#include "truth-control-ids.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
namespace truth {

class Truth4Panel : public rack::app::ModuleWidget {
public:
  explicit Truth4Panel(rack::engine::Module *module) {
    auto constexpr svg_dir = "truth";

    setModule(module);
    setPanel(load_svg(svg_dir, "truth-4"));
    install_screws(this, hp);

    for (int i = 0; i < input_count; i++) {
      auto const input_y = port_top + port_dy * static_cast<float>(i);
      addInput(Jack::input(svg_dir, module, port_x, input_y, Input::Input + i));
      addParam(Button::momentary(svg_dir, module, button_x, input_y,
                                 Param::InputOverride + i));
    }

    for (int i = 0; i < input_count; i++) {
      auto const condition_x =
          condition_left + condition_dx * static_cast<float>(i);
      addParam(Toggle::stepper(svg_dir, "condition", condition_count, module,
                               condition_x, condition_y, Param::Condition + i));
    }

    for (int i = 0; i < pattern_count; i++) {
      auto const outcome_y = outcome_top + outcome_dy * static_cast<float>(i);
      addParam(Toggle::stepper(svg_dir, "outcome", outcome_count, module,
                               outcome_x, outcome_y, Param::Outcome + i));
    }

    addOutput(Jack::output(svg_dir, module, port_x, port_top + port_dy * 4.F,
                           Output::Q));
    addOutput(Jack::output(svg_dir, module, port_x, port_top + port_dy * 5.F,
                           Output::NotQ));
  }

private:
  static auto constexpr input_count = 4;
  static auto constexpr pattern_count = 1 << input_count;

  using Param = ParamIds<input_count>;
  using Input = InputIds<input_count>;
  using Output = OutputIds;

  static auto constexpr hp = 16;
  static auto constexpr condition_count = 5;
  static auto constexpr outcome_count = 2;

  static auto constexpr port_top = 27.94F;
  static auto constexpr port_dy = 15.24F;
  static auto constexpr port_x = 10.16F;
  static auto constexpr button_x = 18.36F;

  static auto constexpr condition_left = 32.796;
  static auto constexpr condition_dx = 9.71F;
  static auto constexpr condition_y = 26.948F;

  static auto constexpr outcome_x = 71.640F;
  static auto constexpr outcome_top = 32.419F;
  static auto constexpr outcome_dy = 4.974F;
};
} // namespace truth
} // namespace dhe
