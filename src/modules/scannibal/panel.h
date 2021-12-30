#pragma once

#include "anchor.h"
#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/shape-controls.h"
#include "controls/step-selection-controls.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "panels/dimensions.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

#include <cmath>
#include <functional>
#include <utility>

namespace dhe {

namespace scannibal {
static auto constexpr base_width_hp = 11.F;
static auto constexpr step_width_hp = 2.25F;
static auto constexpr step_width = hp2mm(step_width_hp);
static auto constexpr section_count = 3;
static auto constexpr global_control_rows = 5;

static auto constexpr top = 23.F;
static auto constexpr bottom = 117.F;

static auto constexpr global_control_width = padding + port_diameter + padding;
static auto constexpr global_inputs_width = global_control_width;
static auto constexpr global_outputs_width = global_control_width;

static auto constexpr labels_width = 10.F;

static auto constexpr small_knob_ascent =
    small_knob_radius + padding + small_font_ascent;
static auto constexpr port_box_descent = port_radius + padding;
static auto constexpr global_controls_top_y = top + small_knob_ascent;
static auto constexpr global_controls_bottom_y = bottom - port_box_descent;
static auto constexpr global_controls_dy =
    (global_controls_bottom_y - global_controls_top_y) /
    static_cast<float>(global_control_rows - 1);

inline auto constexpr global_controls_y(int row) -> float {
  return global_controls_top_y + static_cast<float>(row) * global_controls_dy;
}

inline auto constexpr step_block_width(int steps) -> float {
  return static_cast<float>(steps) * step_width;
}

inline auto constexpr content_width(int steps) -> float {
  return global_inputs_width + labels_width + step_block_width(steps) +
         global_outputs_width;
}

using ProgressLight =
    rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>;

template <typename TSize> struct Panel : public PanelWidget<Panel<TSize>> {
  static auto constexpr step_count = TSize::step_count;
  using InputId = InputIds<step_count>;
  using ParamId = ParamIds<step_count>;
  using LightId = LightIds<step_count>;

  static auto constexpr hp =
      base_width_hp + mm2hp(step_block_width(step_count));
  static auto constexpr panel_file = TSize::panel_file;
  static auto constexpr svg_dir = "scannibal";
  static auto constexpr excess_width =
      hp2mm(hp) - padding - content_width(step_count);
  static auto constexpr progress_light_y = top - light_diameter * 1.5F;

  static auto constexpr margin =
      excess_width / static_cast<float>(section_count + 1);
  static auto constexpr global_inputs_left = margin;
  static auto constexpr step_block_left = global_inputs_left +
                                          global_inputs_width + margin +
                                          labels_width + padding;
  static auto constexpr global_outputs_left =
      step_block_left + step_block_width(step_count) + margin;

  explicit Panel(rack::engine::Module *module)
      : PanelWidget<Panel<TSize>>{module} {
    add_global_inputs();
    add_step_block();
    add_step_selection();
    add_global_outputs();
  }

private:
  void add_global_inputs() {
    auto const x = global_inputs_left + port_radius + padding;
    auto constexpr a_y = global_controls_y(1);
    auto constexpr b_y = global_controls_y(2);
    auto constexpr c_y = global_controls_y(3);
    auto constexpr phase_y = global_controls_y(4);

    InPort::install(this, InputId::InA, x, a_y);
    InPort::install(this, InputId::InB, x, b_y);
    InPort::install(this, InputId::InC, x, c_y);
    InPort::install(this, InputId::Phase, x, phase_y);
  }

  void add_step_block() {
    auto constexpr intra_section_glue = 0.15F;
    auto constexpr inter_section_glue = 2.F;
    auto constexpr stepper_height = padding + small_font_ascent + padding;
    auto constexpr stepper_ascent = stepper_height / 2.F;

    auto constexpr phase_0_anchor_mode_y = top + stepper_ascent;
    auto constexpr phase_0_anchor_source_y =
        phase_0_anchor_mode_y + (stepper_height + stepper_height) / 2.F +
        intra_section_glue;
    auto constexpr phase_0_anchor_level_y =
        phase_0_anchor_source_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;
    auto constexpr phase_0_anchor_level_cv_y =
        phase_0_anchor_level_y + (small_knob_diameter + port_diameter) / 2.F +
        intra_section_glue;

    auto constexpr phase_1_anchor_mode_y =
        phase_0_anchor_level_cv_y + (port_diameter + stepper_height) / 2.F +
        inter_section_glue;
    auto constexpr phase_1_anchor_source_y =
        phase_1_anchor_mode_y + (stepper_height + stepper_height) / 2.F +
        intra_section_glue;
    auto constexpr phase_1_anchor_level_y =
        phase_1_anchor_source_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;
    auto constexpr phase_1_anchor_level_cv_y =
        phase_1_anchor_level_y + (small_knob_diameter + port_diameter) / 2.F +
        intra_section_glue;

    auto constexpr shape_y = phase_1_anchor_level_cv_y +
                             (port_diameter + stepper_height) / 2.F +
                             inter_section_glue;
    auto constexpr curvature_y = shape_y +
                                 (stepper_height + small_knob_diameter) / 2.F +
                                 intra_section_glue;
    auto constexpr curvature_cv_y =
        curvature_y + (small_knob_diameter + port_diameter) / 2.F +
        intra_section_glue;

    auto constexpr duration_y = curvature_cv_y +
                                (port_diameter + small_knob_diameter) / 2.F +
                                inter_section_glue;
    auto constexpr duration_cv_y = duration_y +
                                   (small_knob_diameter + port_diameter) / 2.F +
                                   intra_section_glue;

    for (auto step = 0; step < step_count; step++) {
      auto const step_left =
          step_block_left + static_cast<float>(step) * step_width;
      auto const step_x = step_left + step_width / 2.F;
      this->addChild(rack::createLightCentered<ProgressLight>(
          mm2px(step_x, progress_light_y), this->module,
          LightId::Progress + step + step));

      Stepper::install<AnchorModes>(this, ParamId::StepPhase0AnchorMode + step,
                                    step_x, phase_0_anchor_mode_y);
      Stepper::install<AnchorSources>(this,
                                      ParamId::StepPhase0AnchorSource + step,
                                      step_x, phase_0_anchor_source_y);
      Knob::install<Small>(this, ParamId::StepPhase0AnchorLevel + step, step_x,
                           phase_0_anchor_level_y);
      InPort::install(this, InputId::StepPhase0AnchorLevelCv + step, step_x,
                      phase_0_anchor_level_cv_y);

      Stepper::install<AnchorModes>(this, ParamId::StepPhase1AnchorMode + step,
                                    step_x, phase_1_anchor_mode_y);
      Stepper::install<AnchorSources>(this,
                                      ParamId::StepPhase1AnchorSource + step,
                                      step_x, phase_1_anchor_source_y);
      Knob::install<Small>(this, ParamId::StepPhase1AnchorLevel + step, step_x,
                           phase_1_anchor_level_y);
      InPort::install(this, InputId::StepPhase1AnchorLevelCv + step, step_x,
                      phase_1_anchor_level_cv_y);

      Knob::install<Small>(this, ParamId::StepDuration + step, step_x,
                           duration_y);
      InPort::install(this, InputId::StepDurationCv + step, step_x,
                      duration_cv_y);

      ShapeStepper::install(this, ParamId::StepShape + step, step_x, shape_y);
      Knob::install<Small>(this, ParamId::StepCurvature + step, step_x,
                           curvature_y);
      InPort::install(this, InputId::StepCurvatureCv + step, step_x,
                      curvature_cv_y);
    }
  }

  void add_step_selection() {
    auto const step_x = step_block_left + step_width / 2.F;
    SelectionStartMarker::install(this, step_x - light_diameter,
                                  progress_light_y, step_width);
    auto *end_marker = SelectionEndMarker::install(
        this, step_x + light_diameter, progress_light_y, step_width);

    auto constexpr length_x = global_inputs_left + port_radius + padding;
    auto constexpr length_y = global_controls_y(0);
    SelectionLengthKnob::install<Small>(this, ParamId::SelectionLength,
                                        length_x, length_y, end_marker);
  }

  void add_global_outputs() {
    auto const x = global_outputs_left + port_radius + padding;

    auto const polarity_y = global_controls_y(0);
    auto constexpr step_number_y = global_controls_y(2);
    auto constexpr step_phase_y = global_controls_y(3);
    auto constexpr out_y = global_controls_y(4);

    VoltageRangeSwitch::install(this, ParamId::LevelRange, x, polarity_y);
    OutPort::install(this, OutputIds::StepNumber, x, step_number_y);
    OutPort::install(this, OutputIds::StepPhase, x, step_phase_y);
    OutPort::install(this, OutputIds::Out, x, out_y);
  }
}; // namespace dhe
} // namespace scannibal
} // namespace dhe
