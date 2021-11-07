#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/dimensions.h"
#include "widgets/panel-widget.h"
#include "widgets/step-selection-markers.h"

#include "rack.hpp"

#include <cmath>
#include <functional>
#include <string>
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
  static auto constexpr selection_marker_x = step_block_left + step_width / 2.F;
  static auto constexpr selection_marker_dx = step_width;

  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Param = ParamIds<N>;

  explicit Panel(rack::engine::Module *module)
      : PanelWidget<Panel<TSize>>{module} {

    auto constexpr run_y = global_controls_y(0);
    InPort::install(this, Input::Run, sequence_controls_x, run_y);
    Button::install<Toggle>(this, Param::Run,
                            sequence_controls_x + button_port_distance, run_y);

    auto constexpr loop_y = global_controls_y(1);
    InPort::install(this, Input::Loop, sequence_controls_x, loop_y);
    Button::install<Toggle>(this, Param::Loop,
                            sequence_controls_x + button_port_distance, loop_y);

    auto constexpr progress_light_y = top - light_diameter * 2.F;

    auto *start_marker = StartMarker::install(this, step_block_left,
                                              progress_light_y, step_width);

    auto *end_marker =
        EndMarker::install(this, step_block_left, progress_light_y, step_width);

    auto const update_selection_start = [start_marker, end_marker](int index) {
      start_marker->set_start(index);
      end_marker->set_start(index);
    };

    auto const update_selection_length = [end_marker](int length) {
      end_marker->set_length(length);
    };

    auto constexpr selection_y = global_controls_y(2);
    IntKnob::install<Small>(this, Param::SelectionStart,
                            sequence_controls_x - hp2mm(0.2F), selection_y)
        ->on_change(update_selection_start);

    auto constexpr selection_length_offset = 8.28F;
    auto constexpr selection_length_x =
        sequence_controls_x + selection_length_offset;
    IntKnob::install<Small>(this, Param::SelectionLength, selection_length_x,
                            selection_y)
        ->on_change(update_selection_length);

    auto constexpr gate_y = global_controls_y(3);
    InPort::install(this, Input::Gate, sequence_controls_x, gate_y);
    Button::install<Momentary>(
        this, Param::Gate, sequence_controls_x + button_port_distance, gate_y);

    auto constexpr reset_y = global_controls_y(4);
    InPort::install(this, Input::Reset, sequence_controls_x, reset_y);
    Button::install<Momentary>(this, Param::Reset,
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

    InPort::install(this, Input::LevelAttenuationCV, global_controls_left_x,
                    level_y);
    Knob::install<Small>(this, Param::LevelMultiplier, global_controls_center_x,
                         level_y);
    ThumbSwitch<2>::install(this, Param::LevelRange, global_controls_right_x,
                            level_y);

    InPort::install(this, Input::DurationMultiplierCV, global_controls_left_x,
                    global_duration_y);
    Knob::install<Small>(this, Param::DurationMultiplier,
                         global_controls_center_x, global_duration_y);
    ThumbSwitch<3>::install(this, Param::DurationRange, global_controls_right_x,
                            global_duration_y);

    InPort::install(this, Input::InA, global_controls_left_x, in_y);
    InPort::install(this, Input::InB, global_controls_center_x, in_y);
    InPort::install(this, Input::InC, global_controls_right_x, in_y);

    OutPort::install(this, OutputIds::StepNumber, global_controls_left_x,
                     state_y);
    OutPort::install(this, OutputIds::IsCurving, global_controls_center_x,
                     state_y);
    OutPort::install(this, OutputIds::IsSustaining, global_controls_right_x,
                     state_y);

    OutPort::install(this, OutputIds::StepEventPulse, global_controls_left_x,
                     out_y);
    OutPort::install(this, OutputIds::SequenceEventPulse,
                     global_controls_center_x, out_y);
    OutPort::install(this, OutputIds::Out, global_controls_right_x, out_y);

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
          Light::StepProgress + step + step));

      Stepper<TriggerModes>::install(this, Param::StepTriggerMode + step,
                                     step_x, trigger_y);
      Stepper<InterruptModes>::install(this, Param::StepInterruptMode + step,
                                       step_x, interrupt_y);
      Stepper<SustainModes>::install(this, Param::StepSustainMode + step,
                                     step_x, sustain_y);
      Stepper<AnchorModes>::install(this, Param::StepStartAnchorMode + step,
                                    step_x, start_anchor_mode_y);
      Knob::install<Small>(this, Param::StepStartAnchorLevel + step, step_x,
                           start_anchor_level_y);

      Stepper<AnchorSources>::install(this, Param::StepStartAnchorSource + step,
                                      step_x, start_anchor_source_y);
      Stepper<AnchorModes>::install(this, Param::StepEndAnchorMode + step,
                                    step_x, end_anchor_mode_y);
      Knob::install<Small>(this, Param::StepEndAnchorLevel + step, step_x,
                           end_anchor_level_y);
      Stepper<AnchorSources>::install(this, Param::StepEndAnchorSource + step,
                                      step_x, end_anchor_source_y);

      Knob::install<Small>(this, Param::StepDuration + step, step_x,
                           duration_y);

      Stepper<Shapes>::install(this, Param::StepShape + step, step_x, shape_y);
      Knob::install<Small>(this, Param::StepCurvature + step, step_x,
                           curvature_y);

      Button::install<Toggle>(this, Param::StepEnabled + step, step_x,
                              enabled_y);
    }
  }
}; // namespace dhe
} // namespace sequencizer
} // namespace dhe
