#include "truth-control-ids.h"

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {
namespace truth {

struct Layout {
  int const hp_;
  float const input_top_;
  float const input_port_x_;
  float const input_button_x_;
  float const output_top_;
  float const output_port_x_;
  float const port_dy_;
  float const condition_left_;
  float const condition_dx_;
  float const condition_y_;
  float const outcome_top_;
  float const outcome_dy_;
  float const outcome_x_;
};

static constexpr std::array<Layout, 2> layouts = {
    // 3 inputs
    Layout{
        10,      // NOLINT hp
        76.2F,   // NOLINT input_top
        9.652F,  // NOLINT input_port_x
        17.852F, // NOLINT input_button_x
        90.17F,  // NOLINT output_top
        41.148F, // NOLINT output_port_x
        13.97F,  // NOLINT port_dy
        10.16F,  // NOLINT condition_left
        10.16F,  // NOLINT condition_dx
        26.721F, // NOLINT condition_y_;
        31.75F,  // NOLINT outcome_top;
        4.572F,  // NOLINT outcome_dy;
        40.64F,  // NOLINT outcome_x
    },
    // 4 inputs
    Layout{
        16,      // NOLINT hp
        27.94F,  // NOLINT input_top
        9.652F,  // NOLINT input_port_x
        17.852F, // NOLINT input_button_x
        88.90F,  // NOLINT output_top
        9.652F,  // NOLINT output_port_x
        15.24F,  // NOLINT port_dy
        31.334F, // NOLINT condition_left
        10.16F,  // NOLINT condition_dx
        26.948F, // NOLINT condition_y_;
        32.419F, // NOLINT outcome_top;
        4.974F,  // NOLINT outcome_dy;
        71.974F, // NOLINT outcome_x
    },
};

template <int N> class Panel : public rack::app::ModuleWidget {
public:
  explicit Panel(rack::engine::Module *module) {
    auto constexpr svg_dir = "truth";
    auto const faceplate_filename = std::string{"truth-"} + std::to_string(N);

    setModule(module);
    setPanel(load_svg(svg_dir, faceplate_filename));
    install_screws(this, layout_.hp_);

    for (int i = 0; i < N; i++) {
      auto const input_y =
          layout_.input_top_ + static_cast<float>(i) * layout_.port_dy_;
      addInput(Jack::input(svg_dir, module, layout_.input_port_x_, input_y,
                           Input::Input + i));
      addParam(Button::momentary(svg_dir, module, layout_.input_button_x_,
                                 input_y, Param::InputOverride + i));
    }

    for (int i = 0; i < N; i++) {
      auto const condition_x = layout_.condition_left_ +
                               static_cast<float>(i) * layout_.condition_dx_;
      addParam(Toggle::stepper(svg_dir, "condition", condition_count, module,
                               condition_x, layout_.condition_y_,
                               Param::Condition + i));
    }

    auto constexpr pattern_count = 1 << N;
    for (int i = 0; i < pattern_count; i++) {
      auto const outcome_y =
          layout_.outcome_top_ + layout_.outcome_dy_ * static_cast<float>(i);
      addParam(Toggle::stepper(svg_dir, "outcome", outcome_count, module,
                               layout_.outcome_x_, outcome_y,
                               Param::Outcome + i));
    }

    addOutput(Jack::output(svg_dir, module, layout_.output_port_x_,
                           layout_.output_top_, Output::Q));
    addOutput(Jack::output(svg_dir, module, layout_.output_port_x_,
                           layout_.output_top_ + layout_.port_dy_,
                           Output::NotQ));
  }

private:
  const Layout layout_{layouts[N - 3]};
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
};
} // namespace truth
} // namespace dhe
