#pragma once

#include "advance-mode.h"
#include "control-ids.h"
#include "generate-mode.h"

#include "controls/buttons.h"
#include "controls/duration-controls.h"
#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/shape-controls.h"
#include "controls/step-selection-controls.h"
#include "controls/voltage-controls.h"
#include "panels/panel-widget.h"

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
  static auto constexpr step_count = TSize::step_count; // NOLINT

  static auto constexpr panel_file = TSize::panel_file;
  static auto constexpr svg_dir = "curve-sequencer";
  static auto constexpr step_width_hp = 2.25F;
  static auto constexpr sequence_controls_width_hp = 13.F;
  static auto constexpr hp =
      static_cast<int>(sequence_controls_width_hp + step_count * step_width_hp);
  static auto constexpr step_x = hp2mm(10.F);
  static auto constexpr step_dx = hp2mm(step_width_hp);

  Panel(rack::engine::Module *module) : PanelWidget<Panel<TSize>>{module} {
    using ParamId = ParamIds<step_count>;
    using InputId = InputIds<step_count>;
    using LightId = LightIds<step_count>;

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

    InPort::install(this, InputId::Run, left, run_y);
    Button::install<Toggle>(this, ParamId::Run, left + button_port_distance,
                            run_y);

    InPort::install(this, InputId::Loop, left, loop_y);
    Button::install<Toggle>(this, ParamId::Loop, left + button_port_distance,
                            loop_y);

    auto *start_marker = SelectionStartMarker::install(
        this, step_x - light_diameter, active_y, step_dx);
    auto *end_marker = SelectionEndMarker::install(
        this, step_x + light_diameter, active_y, step_dx);

    SelectionStartKnob::install<Small>(this, ParamId::SelectionStart, left,
                                       selection_y, start_marker, end_marker);

    auto constexpr selection_length_x = left + hp2mm(2.F);
    SelectionLengthKnob::install<Small>(this, ParamId::SelectionLength,
                                        selection_length_x, selection_y,
                                        end_marker);

    InPort::install(this, InputId::Gate, left, gate_y);
    Button::install<Momentary>(this, ParamId::Gate, left + button_port_distance,
                               gate_y);

    InPort::install(this, InputId::Reset, left, reset_y);
    Button::install<Momentary>(this, ParamId::Reset,
                               left + button_port_distance, reset_y);

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

    for (auto step = 0; step < step_count; step++) {
      auto const x = step_x + step_dx * static_cast<float>(step);
      this->addChild(rack::createLightCentered<ProgressLight>(
          mm2px(x, active_y), module, LightId::StepProgress + step + step));

      Stepper::install<GenerateModes>(this, ParamId::StepGenerateMode + step, x,
                                      generate_mode_y);
      Stepper::install<AdvanceModes>(this, ParamId::StepAdvanceMode + step, x,
                                     advance_mode_y);

      Knob::install<Small>(this, ParamId::StepLevel + step, x, level_y);

      ShapeSwitch::install(this, ParamId::StepShape + step, x, shape_y);
      Knob::install<Small>(this, ParamId::StepCurvature + step, x, curve_y);

      Knob::install<Small>(this, ParamId::StepDuration + step, x, duration_y);

      Button::install<Toggle>(this, ParamId::StepEnabled + step, x,
                              enabled_button_y);
      InPort::install(this, InputId::StepEnabled + step, x, enabled_port_y);
    }

    auto constexpr out_y = bottom - port_radius - 1.F;
    auto constexpr eos_y = top + hp2mm(2.75);

    InPort::install(this, InputId::Main, right, eos_y);

    VoltageRangeSwitch::install(this, ParamId::LevelRange, right, level_y);
    DurationRangeSwitch::install(this, ParamId::DurationRange, right,
                                 duration_y);
    OutPort::install(this, OutputId::Main, right, out_y);
  }
}; // namespace dhe
} // namespace curve_sequencer
} // namespace dhe
