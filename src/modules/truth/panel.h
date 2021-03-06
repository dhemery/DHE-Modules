#include "control-ids.h"

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {
namespace truth {

static auto constexpr condition_dx = 10.16F;
static auto constexpr outcome_dy = 5.08F;

struct Layout {
  int hp_;
  float condition_y_;
  float outcome_x_;
  float input_top_;
  float input_x_;
  float output_top_;
  float output_x_;
  float port_dy_;
};

static auto layout(int input_count) -> Layout {
  auto l = Layout{};
  if (input_count == 2) {
    auto constexpr input_top = 58.F;
    auto constexpr port_dy = 15.5F;
    auto constexpr hp = 8;
    auto constexpr port_offset = 3.5F;
    auto constexpr center_x = 20.48F;
    l.hp_ = hp;
    l.condition_y_ = 21.F;
    l.outcome_x_ = 30.48F;
    l.port_dy_ = port_dy;
    l.input_top_ = input_top;
    l.input_x_ = center_x - port_offset;
    l.output_top_ = input_top + 2.F * port_dy;
    l.output_x_ = center_x + port_offset;
  } else if (input_count == 3) {
    auto constexpr input_top = 76.F;
    auto constexpr port_dy = 15.5F;
    auto constexpr hp = 10;
    l.hp_ = hp;
    l.condition_y_ = 20.F;
    l.outcome_x_ = 40.64F;
    l.port_dy_ = port_dy;
    l.input_top_ = input_top;
    l.input_x_ = 10.28F;
    l.output_top_ = input_top + port_dy;
    l.output_x_ = 40.64F;
  } else {
    auto constexpr hp = 16;
    auto constexpr input_top = 29.36F;
    auto constexpr input_x = 10.28F;
    auto constexpr port_dy = 14.585F;
    l.hp_ = hp;
    l.condition_y_ = 23.66F;
    l.outcome_x_ = 71.11F;
    l.port_dy_ = port_dy;
    l.input_top_ = input_top;
    l.input_x_ = 10.28F;
    l.output_top_ = input_top + 4.F * port_dy;
    l.output_x_ = input_x + 7.F;
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
      addInput(
          Jack::input(svg_dir, module, layout_.input_x_, y, Input::Input + i));
      addParam(Button::momentary(svg_dir, module,
                                 layout_.input_x_ + button_port_distance, y,
                                 Param::InputOverride + i));
    }

    auto const condition_y = layout_.condition_y_;
    auto const gate_mode_x = layout_.outcome_x_ - condition_dx;
    addParam(Toggle::stepper(svg_dir, "gate-mode", gate_mode_count, module,
                             gate_mode_x, condition_y, Param::GateMode));

    auto constexpr pattern_count = 1 << N;
    auto const outcome_top = layout_.condition_y_ + outcome_dy;
    for (int i = 0; i < pattern_count; i++) {
      auto const y = outcome_top + static_cast<float>(i) * outcome_dy;
      addParam(Toggle::stepper(svg_dir, "outcome", outcome_count, module,
                               layout_.outcome_x_, y, Param::Outcome + i));
    }

    addParam(Button::output(svg_dir, module,
                            layout_.output_x_ - button_port_distance,
                            layout_.output_top_, Param::QOverride));
    addOutput(Jack::output(svg_dir, module, layout_.output_x_,
                           layout_.output_top_, Output::Q));
    addParam(Button::output(
        svg_dir, module, layout_.output_x_ - button_port_distance,
        layout_.output_top_ + layout_.port_dy_, Param::QNotOverride));
    addOutput(Jack::output(svg_dir, module, layout_.output_x_,
                           layout_.output_top_ + layout_.port_dy_,
                           Output::QNot));
  }

private:
  const Layout layout_{layout(N)};
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
};
} // namespace truth
} // namespace dhe
