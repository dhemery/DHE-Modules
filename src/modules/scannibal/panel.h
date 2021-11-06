#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/dimensions.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

#include <cmath>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace dhe {

namespace scannibal {
static auto constexpr base_width_hp = 11.F;
static auto constexpr step_width_hp = 2.25F;
static auto constexpr section_count = 3;
static auto constexpr global_control_rows = 5;

static auto constexpr step_width = hp2mm(step_width_hp);
static auto constexpr top = 23.F;
static auto constexpr bottom = 117.F;

static auto constexpr global_control_width = padding + port_diameter + padding;
static auto constexpr global_inputs_width = global_control_width;
static auto constexpr global_outputs_width = global_control_width;

static auto constexpr labels_width = 10.F;

static auto constexpr port_box_ascent =
    padding + small_label_size + port_radius + 0.22F;
static auto constexpr port_box_descent = padding + port_radius;
static auto constexpr global_controls_top_y = top + port_box_ascent;
static auto constexpr global_controls_bottom_y = bottom - port_box_descent;
static auto constexpr global_controls_dy =
    (global_controls_bottom_y - global_controls_top_y) /
    static_cast<float>(global_control_rows - 1);

static inline auto constexpr global_controls_y(int row) -> float {
  return global_controls_top_y + static_cast<float>(row) * global_controls_dy;
}

static inline auto constexpr step_block_width(int steps) -> float {
  return static_cast<float>(steps) * step_width;
}

static inline auto constexpr content_width(int steps) -> float {
  return global_inputs_width + labels_width + step_block_width(steps) +
         global_outputs_width;
}

using ProgressLight =
    rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>;

template <typename TSize> struct Panel : public PanelWidget<Panel<TSize>> {
  static auto constexpr N = TSize::step_count;
  static auto constexpr hp = base_width_hp + mm2hp(step_block_width(N));
  static auto constexpr panel_file = TSize::panel_file;
  static auto constexpr svg_dir = "scannibal";

  using Input = InputIds<N>;
  using Param = ParamIds<N>;
  using Light = LightIds<N>;

  explicit Panel(rack::engine::Module *module)
      : PanelWidget<Panel<TSize>>{module} {

    auto constexpr excess_width = hp2mm(hp) - padding - content_width(N);
    auto constexpr margin =
        excess_width / static_cast<float>(section_count + 1);

    auto constexpr global_inputs_left = margin;
    add_global_inputs(global_inputs_left);

    auto constexpr step_block_left = global_inputs_left + global_inputs_width +
                                     margin + labels_width + padding;
    add_step_block(step_block_left);

    auto constexpr global_outputs_left =
        step_block_left + step_block_width(N) + margin;
    add_global_outputs(global_outputs_left);
  }

private:
  rack::widget::SvgWidget *start_marker_ = new rack::widget::SvgWidget;
  rack::widget::SvgWidget *end_marker_ = new rack::widget::SvgWidget;
  float end_marker_x_;

  void add_global_inputs(float left) {
    auto const x = left + port_radius + padding;
    auto constexpr length_y = global_controls_y(0);
    auto constexpr a_y = global_controls_y(1);
    auto constexpr b_y = global_controls_y(2);
    auto constexpr c_y = global_controls_y(3);
    auto constexpr phase_y = global_controls_y(4);

    auto const update_selection_length = [this](int step) {
      set_selection_length(step);
    };
    Knob::install<Small, int>(this, Param::Length, x, length_y)
        ->on_change(update_selection_length);

    InPort::install(this, Input::InA, x, a_y);
    InPort::install(this, Input::InB, x, b_y);
    InPort::install(this, Input::InC, x, c_y);
    InPort::install(this, Input::Phase, x, phase_y);
  }

  void add_step_block(float left) {
    auto constexpr progress_light_y = top - light_diameter * 1.5F;
    auto constexpr intra_section_glue = 0.15F;
    auto constexpr inter_section_glue = 2.F;
    auto constexpr stepper_ascent = small_label_size / 2.F + padding - 0.25F;
    auto constexpr stepper_height = stepper_ascent * 2.F;

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

    auto const start_marker_x = left + step_width / 2.F - light_diameter;
    start_marker_->setSvg(load_svg(svg_dir, "marker-start"));
    position_centered(start_marker_, start_marker_x, progress_light_y);
    this->addChild(start_marker_);

    end_marker_x_ = left + step_width / 2.F;
    end_marker_->setSvg(load_svg(svg_dir, "marker-end"));
    position_centered(end_marker_, 0.F, progress_light_y);
    this->addChild(end_marker_);
    set_selection_length(N);

    for (auto step = 0; step < N; step++) {
      auto const step_left = left + static_cast<float>(step) * step_width;
      auto const step_x = step_left + step_width / 2.F;
      this->addChild(rack::createLightCentered<ProgressLight>(
          mm2px(step_x, progress_light_y), this->module,
          Light::Progress + step + step));

      Stepper<AnchorModes>::install(this, Param::StepPhase0AnchorMode + step,
                                    step_x, phase_0_anchor_mode_y);
      Stepper<AnchorSources>::install(this,
                                      Param::StepPhase0AnchorSource + step,
                                      step_x, phase_0_anchor_source_y);
      Knob::install<Small>(this, Param::StepPhase0AnchorLevel + step, step_x,
                           phase_0_anchor_level_y);
      InPort::install(this, Input::StepPhase0AnchorLevelCv + step, step_x,
                      phase_0_anchor_level_cv_y);

      Stepper<AnchorModes>::install(this, Param::StepPhase1AnchorMode + step,
                                    step_x, phase_1_anchor_mode_y);
      Stepper<AnchorSources>::install(this,
                                      Param::StepPhase1AnchorSource + step,
                                      step_x, phase_1_anchor_source_y);
      Knob::install<Small>(this, Param::StepPhase1AnchorLevel + step, step_x,
                           phase_1_anchor_level_y);
      InPort::install(this, Input::StepPhase1AnchorLevelCv + step, step_x,
                      phase_1_anchor_level_cv_y);

      Knob::install<Small>(this, Param::StepDuration + step, step_x,
                           duration_y);
      InPort::install(this, Input::StepDurationCv + step, step_x,
                      duration_cv_y);

      Stepper<Shapes>::install(this, Param::StepShape + step, step_x, shape_y);
      Knob::install<Small>(this, Param::StepCurvature + step, step_x,
                           curvature_y);
      InPort::install(this, Input::StepCurvatureCv + step, step_x,
                      curvature_cv_y);
    }
  }

  void add_global_outputs(float left) {
    auto const x = left + port_radius + padding;

    auto const polarity_y = global_controls_y(0);
    auto constexpr step_number_y = global_controls_y(2);
    auto constexpr step_phase_y = global_controls_y(3);
    auto constexpr out_y = global_controls_y(4);

    ThumbSwitch<2>::install(this, Param::LevelRange, x, polarity_y);
    OutPort::install(this, OutputIds::StepNumber, x, step_number_y);
    OutPort::install(this, OutputIds::StepPhase, x, step_phase_y);
    OutPort::install(this, OutputIds::Out, x, out_y);
  }

  void set_selection_length(int length) {
    auto const x = end_marker_x_ + step_width * static_cast<float>(length - 1);
    end_marker_->box.pos.x = mm2px(x);
  }
}; // namespace dhe
} // namespace scannibal
} // namespace dhe
