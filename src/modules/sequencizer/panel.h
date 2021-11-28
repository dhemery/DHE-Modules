#pragma once

#include "anchor.h"
#include "interrupter.h"
#include "sustainer.h"
#include "trigger-mode.h"

#include "control-ids.h"
#include "controls/buttons.h"
#include "controls/duration-controls.h"
#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/step-selection-controls.h"
#include "controls/switches.h"
#include "signals/voltage-signals.h"
#include "widgets/dimensions.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

#include <cmath>
#include <functional>
#include <utility>

namespace dhe {

namespace sequencizer {
auto constexpr base_width_hp = 15.F;
static auto constexpr step_width_hp = 2.25F;
static auto constexpr step_width = hp2mm(step_width_hp);
static auto constexpr top = 23.F;
static auto constexpr bottom = 117.F;

static auto constexpr sequence_controls_width =
    padding + port_diameter + padding + button_diameter + padding;

static auto constexpr global_control_width = padding + port_diameter + padding;
static auto constexpr global_control_columns = 3.F;
static auto constexpr global_controls_width =
    global_control_width * global_control_columns +
    padding * (global_control_columns - 1);

static auto constexpr labels_width = 10.F;

static auto constexpr port_box_ascent =
    padding + small_label_size + port_radius + 0.22F;
static auto constexpr port_box_descent = padding + port_radius;
static auto constexpr global_controls_top_y = top + port_box_ascent;
static auto constexpr global_controls_bottom_y = bottom - port_box_descent;
static auto constexpr global_controls_dy =
    (global_controls_bottom_y - global_controls_top_y) / 4.F;

static inline auto constexpr global_controls_y(int row) -> float {
  return global_controls_top_y + static_cast<float>(row) * global_controls_dy;
}

using ProgressLight =
    rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>;

template <typename TSize> struct Panel : public PanelWidget<Panel<TSize>> {
  static auto constexpr N = TSize::step_count;
  static auto constexpr step_block_width_hp = N * step_width_hp;
  static auto constexpr hp = base_width_hp + step_block_width_hp;
  static auto constexpr panel_file = TSize::panel_file;
  static auto constexpr svg_dir = "sequencizer";
  static auto constexpr content_width = sequence_controls_width +
                                        global_controls_width + labels_width +
                                        hp2mm(step_block_width_hp);
  static auto constexpr excess_width = hp2mm(hp) - padding - content_width;
  static auto constexpr margin = excess_width / 4.F;
  static auto constexpr sequence_controls_left = margin;
  static auto constexpr global_controls_left =
      sequence_controls_left + sequence_controls_width + margin;
  static auto constexpr step_block_left = global_controls_left +
                                          global_controls_width + margin +
                                          labels_width + padding;
  static auto constexpr sequence_controls_x =
      sequence_controls_left + padding + port_radius;

  using ParamId = ParamIds<N>;
  using LightId = LightIds<N>;

  explicit Panel(rack::engine::Module *module)
      : PanelWidget<Panel<TSize>>{module} {

    auto constexpr run_y = global_controls_y(0);
    InPort::install(this, InputId::Run, sequence_controls_x, run_y);
    Button::install<Toggle>(this, ParamId::Run,
                            sequence_controls_x + button_port_distance, run_y);

    auto constexpr loop_y = global_controls_y(1);
    InPort::install(this, InputId::Loop, sequence_controls_x, loop_y);
    Button::install<Toggle>(this, ParamId::Loop,
                            sequence_controls_x + button_port_distance, loop_y);

    auto constexpr progress_light_y = top - light_diameter * 2.F;

    auto const positions = SelectionMarkerPositions{
        step_block_left + step_width / 2.F, progress_light_y, step_width};
    auto *start_marker = SelectionStartMarker::install(this, positions);
    auto *end_marker = SelectionEndMarker::install(this, positions);

    auto constexpr selection_y = global_controls_y(2);
    SelectionStartKnob::install<Small>(this, ParamId::SelectionStart,
                                       sequence_controls_x - hp2mm(0.2F),
                                       selection_y, start_marker, end_marker);

    auto constexpr selection_length_offset = 8.28F;
    auto constexpr selection_length_x =
        sequence_controls_x + selection_length_offset;
    SelectionLengthKnob::install<Small>(this, ParamId::SelectionLength,
                                        selection_length_x, selection_y,
                                        end_marker);

    auto constexpr gate_y = global_controls_y(3);
    InPort::install(this, InputId::Gate, sequence_controls_x, gate_y);
    Button::install<Momentary>(this, ParamId::Gate,
                               sequence_controls_x + button_port_distance,
                               gate_y);

    auto constexpr reset_y = global_controls_y(4);
    InPort::install(this, InputId::Reset, sequence_controls_x, reset_y);
    Button::install<Momentary>(this, ParamId::Reset,
                               sequence_controls_x + button_port_distance,
                               reset_y);

    auto constexpr level_y = global_controls_y(0);
    auto constexpr global_duration_y = global_controls_y(1);
    auto constexpr in_y = global_controls_y(2);
    auto constexpr state_y = global_controls_y(3);
    auto constexpr out_y = global_controls_y(4);

    auto constexpr global_controls_left_x =
        global_controls_left + global_control_width / 2.F;
    auto constexpr global_controls_center_x =
        global_controls_left_x + global_control_width + padding;
    auto constexpr global_controls_right_x =
        global_controls_center_x + global_control_width + padding;

    InPort::install(this, InputId::LevelAttenuationCV, global_controls_left_x,
                    level_y);
    Knob::install<Small>(this, ParamId::LevelMultiplier,
                         global_controls_center_x, level_y);
    VoltageRangeSwitch::install(this, ParamId::LevelRange,
                                global_controls_right_x, level_y);

    InPort::install(this, InputId::DurationMultiplierCV, global_controls_left_x,
                    global_duration_y);
    Knob::install<Small>(this, ParamId::DurationMultiplier,
                         global_controls_center_x, global_duration_y);
    DurationRangeSwitch::install(this, ParamId::DurationRange,
                                 global_controls_right_x, global_duration_y);

    InPort::install(this, InputId::InA, global_controls_left_x, in_y);
    InPort::install(this, InputId::InB, global_controls_center_x, in_y);
    InPort::install(this, InputId::InC, global_controls_right_x, in_y);

    OutPort::install(this, OutputId::StepNumber, global_controls_left_x,
                     state_y);
    OutPort::install(this, OutputId::IsCurving, global_controls_center_x,
                     state_y);
    OutPort::install(this, OutputId::IsSustaining, global_controls_right_x,
                     state_y);

    OutPort::install(this, OutputId::StepEventPulse, global_controls_left_x,
                     out_y);
    OutPort::install(this, OutputId::SequenceEventPulse,
                     global_controls_center_x, out_y);
    OutPort::install(this, OutputId::Out, global_controls_right_x, out_y);

    auto constexpr intra_section_glue = 0.5F;
    auto constexpr inter_section_glue = 4.F;
    auto constexpr stepper_ascent = small_label_size / 2.F + padding - 0.25F;
    auto constexpr stepper_height = stepper_ascent * 2.F;

    auto constexpr trigger_y = top + stepper_ascent;
    auto constexpr interrupt_y =
        trigger_y + stepper_height + intra_section_glue;
    auto constexpr sustain_y =
        interrupt_y + stepper_height + intra_section_glue;

    auto constexpr start_anchor_mode_y =
        sustain_y + stepper_height + inter_section_glue;
    auto constexpr start_anchor_level_y =
        start_anchor_mode_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;
    auto constexpr start_anchor_source_y =
        start_anchor_level_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;

    auto constexpr end_anchor_mode_y =
        start_anchor_source_y + stepper_height + inter_section_glue;
    auto constexpr end_anchor_level_y =
        end_anchor_mode_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;
    auto constexpr end_anchor_source_y =
        end_anchor_level_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;

    auto constexpr duration_y = end_anchor_source_y +
                                (stepper_height + small_knob_diameter) / 2.F +
                                inter_section_glue;

    auto constexpr shape_y = duration_y +
                             (stepper_height + small_knob_diameter) / 2.F +
                             inter_section_glue;
    auto constexpr curvature_y = shape_y +
                                 (stepper_height + small_knob_diameter) / 2.F +
                                 intra_section_glue;

    auto constexpr enabled_y = bottom - button_radius;

    for (auto step = 0; step < N; step++) {
      auto const step_left =
          step_block_left + static_cast<float>(step) * step_width;
      auto const step_x = step_left + step_width / 2.F;
      this->addChild(rack::createLightCentered<ProgressLight>(
          mm2px(step_x, progress_light_y), module,
          LightId::StepProgress + step + step));

      Stepper::install<TriggerModes>(this, ParamId::StepTriggerMode + step,
                                     step_x, trigger_y);
      Stepper::install<InterruptModes>(this, ParamId::StepInterruptMode + step,
                                       step_x, interrupt_y);
      Stepper::install<SustainModes>(this, ParamId::StepSustainMode + step,
                                     step_x, sustain_y);
      Stepper::install<AnchorModes>(this, ParamId::StepStartAnchorMode + step,
                                    step_x, start_anchor_mode_y);
      Knob::install<Small>(this, ParamId::StepStartAnchorLevel + step, step_x,
                           start_anchor_level_y);

      Stepper::install<AnchorSources>(this,
                                      ParamId::StepStartAnchorSource + step,
                                      step_x, start_anchor_source_y);
      Stepper::install<AnchorModes>(this, ParamId::StepEndAnchorMode + step,
                                    step_x, end_anchor_mode_y);
      Knob::install<Small>(this, ParamId::StepEndAnchorLevel + step, step_x,
                           end_anchor_level_y);
      Stepper::install<AnchorSources>(this, ParamId::StepEndAnchorSource + step,
                                      step_x, end_anchor_source_y);

      Knob::install<Small>(this, ParamId::StepDuration + step, step_x,
                           duration_y);

      ShapeStepper::install(this, ParamId::StepShape + step, step_x, shape_y);
      Knob::install<Small>(this, ParamId::StepCurvature + step, step_x,
                           curvature_y);

      Button::install<Toggle>(this, ParamId::StepEnabled + step, step_x,
                              enabled_y);
    }
  }
}; // namespace dhe
} // namespace sequencizer
} // namespace dhe
