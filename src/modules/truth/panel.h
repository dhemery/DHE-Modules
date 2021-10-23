#pragma once

#include "control-ids.h"
#include "controls.h"

#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace truth {

struct Truth2 {
  static auto constexpr hp = 8;
  static auto constexpr input_count = 2;
  static auto constexpr panel_file = "truth-2";

  static auto constexpr condition_y = 21.F;
  static auto constexpr center_x = 20.48F;
  static auto constexpr input_top = 58.F;
  static auto constexpr outcome_x = 30.48F;
  static auto constexpr port_dy = 15.5F;
  static auto constexpr port_offset = 3.5F;

  static auto constexpr input_x = center_x - port_offset;
  static auto constexpr output_top = input_top + 2.F * port_dy;
  static auto constexpr output_x = center_x + port_offset;
};

struct Truth3 {
  static auto constexpr hp = 10;
  static auto constexpr input_count = 3;
  static auto constexpr panel_file = "truth-3";

  static auto constexpr condition_y = 20.F;
  static auto constexpr input_x = 10.28F;
  static auto constexpr input_top = 76.F;
  static auto constexpr outcome_x = 40.64F;
  static auto constexpr output_x = 40.64F;
  static auto constexpr port_dy = 15.5F;

  static auto constexpr output_top = input_top + port_dy;
};

struct Truth4 {
  static auto constexpr hp = 16;
  static auto constexpr input_count = 4;
  static auto constexpr panel_file = "truth-4";

  static auto constexpr condition_y = 23.66F;
  static auto constexpr input_top = 29.36F;
  static auto constexpr input_x = 10.28F;
  static auto constexpr outcome_x = 71.11F;
  static auto constexpr port_dy = 14.585F;

  static auto constexpr output_top = input_top + 4.F * port_dy;
  static auto constexpr output_x = input_x + 7.F;
};

template <typename TLayout> struct Panel : public PanelWidget<Panel<TLayout>> {
  static auto constexpr hp = TLayout::hp;
  static auto constexpr panel_file = TLayout::panel_file;
  static auto constexpr svg_dir = "truth";

  explicit Panel(rack::engine::Module *module)
      : PanelWidget<Panel<TLayout>>{module} {
    static auto constexpr input_count = TLayout::input_count;
    using Input = InputIds<input_count>;
    using Output = OutputIds;
    using Param = ParamIds<input_count>;

    static auto constexpr condition_dx = 10.16F;
    static auto constexpr outcome_dy = 5.08F;

    for (int i = 0; i < input_count; i++) {
      auto const y =
          TLayout::input_top + static_cast<float>(i) * TLayout::port_dy;
      InPort::install(this, Input::Input + i, TLayout::input_x, y);
      Button::install<Momentary>(this, Param::InputOverride + i,
                                 TLayout::input_x + button_port_distance, y);
    }

    auto const condition_y = TLayout::condition_y;
    auto const gate_mode_x = TLayout::outcome_x - condition_dx;
    Stepper<GateModes>::install(this, Param::GateMode, gate_mode_x,
                                condition_y);

    auto constexpr pattern_count = 1 << input_count;
    auto const outcome_top = TLayout::condition_y + outcome_dy;
    for (int i = 0; i < pattern_count; i++) {
      auto const y = outcome_top + static_cast<float>(i) * outcome_dy;
      Stepper<Outcomes>::install(this, Param::Outcome + i, TLayout::outcome_x,
                                 y);
    }

    Button::install<Momentary, Reversed>(
        this, Param::QOverride, TLayout::output_x - button_port_distance,
        TLayout::output_top);
    OutPort::install(this, Output::Q, TLayout::output_x, TLayout::output_top);
    Button::install<Momentary, Reversed>(
        this, Param::QNotOverride, TLayout::output_x - button_port_distance,
        TLayout::output_top + TLayout::port_dy);
    OutPort::install(this, Output::QNot, TLayout::output_x,
                     TLayout::output_top + TLayout::port_dy);
  }
};
} // namespace truth
} // namespace dhe
