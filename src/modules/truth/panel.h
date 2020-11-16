#include "truth-control-ids.h"

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {
namespace truth {

auto constexpr condition_dx = 10.16F;
auto constexpr outcome_dy = 5.08F;
auto constexpr input_x = 10.28;
auto constexpr input_button_x = input_x + button_port_distance;

struct Layout {
  int hp_;
  float condition_y_;
  float outcome_x_;
  float input_top_;
  float output_top_;
  float output_x_;
  float port_dy_;
};

static auto layout(int input_count) -> Layout {
  auto l = Layout{};
  if (input_count == 3) {
    auto constexpr input_top = 80.F;
    auto constexpr port_dy = 14.5F;
    auto constexpr hp = 10;
    l.hp_ = hp;
    l.condition_y_ = 23.F;
    l.outcome_x_ = 40.64F;
    l.port_dy_ = port_dy;
    l.input_top_ = input_top;
    l.output_top_ = input_top + port_dy;
    l.output_x_ = 40.64F;
  } else {
    auto constexpr hp = 16;
    auto constexpr input_top = 26.36F;
    auto constexpr port_dy = 15.6F;
    l.hp_ = hp;
    l.condition_y_ = 25.74F;
    l.outcome_x_ = 71.11F;
    l.port_dy_ = port_dy;
    l.input_top_ = input_top;
    l.output_top_ = input_top + 4.F * port_dy;
    l.output_x_ = input_x;
  }
  return l;
}

template <int N> class Panel : public rack::app::ModuleWidget {
public:
  explicit Panel(rack::engine::Module *module) {
    auto constexpr svg_dir = "truth";
    auto const faceplate_filename = std::string{"truth-"} + std::to_string(N);

    setModule(module);
    setPanel(load_svg(svg_dir, faceplate_filename));
    install_screws(this, layout_.hp_);

    for (int i = 0; i < N; i++) {
      auto const y =
          layout_.input_top_ + static_cast<float>(i) * layout_.port_dy_;
      addInput(Jack::input(svg_dir, module, input_x, y, Input::Input + i));
      addParam(Button::momentary(svg_dir, module, input_button_x, y,
                                 Param::InputOverride + i));
    }

    auto const condition_left =
        layout_.outcome_x_ - static_cast<float>(N) * condition_dx;
    auto const condition_y = layout_.condition_y_;
    for (int i = 0; i < N; i++) {
      auto const x = condition_left + static_cast<float>(i) * condition_dx;
      addParam(Toggle::stepper(svg_dir, "condition", condition_count, module, x,
                               condition_y, Param::Condition + i));
    }

    auto constexpr pattern_count = 1 << N;
    auto const outcome_top = layout_.condition_y_ + outcome_dy;
    for (int i = 0; i < pattern_count; i++) {
      auto const y = outcome_top + static_cast<float>(i) * outcome_dy;
      addParam(Toggle::stepper(svg_dir, "outcome", outcome_count, module,
                               layout_.outcome_x_, y, Param::Outcome + i));
    }

    addOutput(Jack::output(svg_dir, module, layout_.output_x_,
                           layout_.output_top_, Output::Q));
    addOutput(Jack::output(svg_dir, module, layout_.output_x_,
                           layout_.output_top_ + layout_.port_dy_,
                           Output::NotQ));
  }

private:
  const Layout layout_{layout(N)};
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
};
} // namespace truth
} // namespace dhe
