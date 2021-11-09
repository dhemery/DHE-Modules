#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "widgets/panel-widget.h"
#include "widgets/step-selection-markers.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace dhe {

namespace curve_sequencer {

using ProgressLight =
    rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>;

template <typename TSize> struct Panel : public PanelWidget<Panel<TSize>> {
  static auto constexpr N = TSize::step_count; // NOLINT
  static auto constexpr panel_file = TSize::panel_file;
  static auto constexpr svg_dir = "curve-sequencer";
  static auto constexpr step_width_hp = 2.25F;
  static auto constexpr sequence_controls_width_hp = 13.F;
  static auto constexpr hp =
      static_cast<int>(sequence_controls_width_hp + N * step_width_hp);
  static auto constexpr step_x = hp2mm(10.F);
  static auto constexpr step_dx = hp2mm(step_width_hp);

  Panel(rack::engine::Module *module) : PanelWidget<Panel<TSize>>{module} {
    auto constexpr left = hp2mm(2.F);
    auto constexpr right = hp2mm(hp - 2.F);
    auto constexpr top = hp2mm(4.F);
    auto constexpr bottom = hp2mm(23);

    auto constexpr input_top = top + hp2mm(2.75);
    auto constexpr input_bottom = bottom - port_radius - 1.F;
    auto constexpr input_dy = (input_bottom - input_top) / 4.F;

    auto constexpr run_y = input_top + 0.F * input_dy;
    auto constexpr loop_y = input_top + 1.F * input_dy;
    auto constexpr selection_y = input_top + 2.F * input_dy;
    auto constexpr gate_y = input_top + 3.F * input_dy;
    auto constexpr reset_y = input_top + 4.F * input_dy;

    auto constexpr active_y = top + light_radius;

    InPort::install(this, Input::Run, left, run_y);
    Button::install<Toggle>(this, Param::Run, left + button_port_distance,
                            run_y);

    InPort::install(this, Input::Loop, left, loop_y);
    Button::install<Toggle>(this, Param::Loop, left + button_port_distance,
                            loop_y);

    auto constexpr marker_x = step_x;
    auto *start_marker =
        StartMarker::install(this, marker_x, active_y, step_dx);
    auto *end_marker = EndMarker::install(this, marker_x, active_y, step_dx);

    auto const update_selection_start = [start_marker, end_marker](int index) {
      start_marker->set_start(index);
      end_marker->set_start(index);
    };
    IntKnob::install<Small>(this, Param::SelectionStart, left, selection_y)
        ->on_change(update_selection_start);

    auto const update_selection_length = [end_marker](int offset) {
      end_marker->set_length(offset);
    };
    auto constexpr selection_length_x = left + hp2mm(2.F);
    IntKnob::install<Small>(this, Param::SelectionLength, selection_length_x,
                            selection_y)
        ->on_change(update_selection_length);

    InPort::install(this, Input::Gate, left, gate_y);
    Button::install<Momentary>(this, Param::Gate, left + button_port_distance,
                               gate_y);

    InPort::install(this, Input::Reset, left, reset_y);
    Button::install<Momentary>(this, Param::Reset, left + button_port_distance,
                               reset_y);

    auto constexpr generate_mode_y = top + hp2mm(1.61F);
    auto constexpr advance_mode_y = top + hp2mm(3.25F);
    auto constexpr level_y = top + hp2mm(5.75F);
    auto constexpr shape_y = top + hp2mm(8.75F);
    auto constexpr curve_y = top + hp2mm(11.0F);
    auto constexpr duration_y = top + hp2mm(14.0F);
    auto constexpr enabled_port_y = bottom - port_radius;
    auto constexpr enabled_button_y =
        enabled_port_y - port_radius - button_radius - 1.F;

    auto const generate_mode_labels = std::vector<std::string>{
        "curve", "hold", "sustain", "input", "chase", "level"};
    auto const advance_mode_labels =
        std::vector<std::string>{"time", "rise", "fall", "edge", "high", "low"};

    for (auto step = 0; step < N; step++) {
      auto const x = step_x + step_dx * (float)step;
      this->addChild(rack::createLightCentered<ProgressLight>(
          mm2px(x, active_y), module, Light::StepProgress + step + step));

      Stepper::install<GenerateModes>(this, Param::StepGenerateMode + step, x,
                                      generate_mode_y);
      Stepper::install<AdvanceModes>(this, Param::StepAdvanceMode + step, x,
                                     advance_mode_y);

      Knob::install<Small>(this, Param::StepLevel + step, x, level_y);

      ThumbSwitch::install<Shapes>(this, Param::StepShape + step, x, shape_y);
      Knob::install<Small>(this, Param::StepCurvature + step, x, curve_y);

      Knob::install<Small>(this, Param::StepDuration + step, x, duration_y);

      Button::install<Toggle>(this, Param::StepEnabled + step, x,
                              enabled_button_y);
      InPort::install(this, Input::StepEnabled + step, x, enabled_port_y);
    }

    auto constexpr out_y = bottom - port_radius - 1.F;
    auto constexpr eos_y = top + hp2mm(2.75);

    InPort::install(this, Input::Main, right, eos_y);

    ThumbSwitch::install<Voltages>(this, Param::LevelRange, right, level_y);
    ThumbSwitch::install<Durations>(this, Param::DurationRange, right,
                                    duration_y);
    OutPort::install(this, Output::Main, right, out_y);
  }

private:
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
}; // namespace dhe
} // namespace curve_sequencer
} // namespace dhe
