#pragma once

#include "control-ids.h"
#include "widgets/control-widgets.h"
#include "widgets/dimensions.h"
#include "widgets/screws.h"
#include <app/ModuleWidget.hpp>
#include <cmath>
#include <componentlibrary.hpp>
#include <functional>
#include <jansson.h>
#include <string>
#include <utility>

namespace dhe {

namespace phase_sequencer {
static auto constexpr base_width_hp = 11.F;
static auto constexpr step_width_hp = 2.25F;
static auto constexpr section_count = 3;
static auto constexpr global_control_rows = 5;

static auto constexpr step_width = hp2mm(step_width_hp);
static auto constexpr top = 23.F;
static auto constexpr bottom = 117.F;

static auto constexpr sequence_controls_width =
    padding + port_diameter + padding + button_diameter + padding;

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

template <int N> class Panel : public rack::app::ModuleWidget {
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;

public:
  Panel(rack::engine::Module *module) {

    auto constexpr hp = base_width_hp + mm2hp(step_block_width(N));

    setModule(module);
    setPanel(background_svg(slug_));
    install_screws(this, static_cast<int>(hp));

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
  const std::string slug_ = std::string{"phase-sequencer-"} + std::to_string(N);

  void add_global_inputs(float left) {
    auto const x = left + port_radius + padding;
    auto const polarity_y = global_controls_y(0);
    auto const a_y = global_controls_y(1);
    auto const b_y = global_controls_y(2);
    auto const c_y = global_controls_y(3);
    auto const phase_y = global_controls_y(4);

    addParam(Toggle::thumb(2, slug_, module, x, polarity_y, Param::LevelRange));
    addInput(Jack::input(slug_, module, x, a_y, Input::InA));
    addInput(Jack::input(slug_, module, x, b_y, Input::InB));
    addInput(Jack::input(slug_, module, x, c_y, Input::InC));
    addInput(Jack::input(slug_, module, x, phase_y, Input::Phase));
  }
  void add_step_block(float left) {
    auto constexpr progress_light_y = top - light_diameter * 1.5F;
    auto constexpr intra_section_glue = 0.2F;
    auto constexpr inter_section_glue = 2.F;
    auto constexpr stepper_ascent = small_label_size / 2.F + padding - 0.25F;
    auto constexpr stepper_height = stepper_ascent * 2.F;

    auto constexpr start_anchor_mode_y = top + stepper_ascent;
    auto constexpr start_anchor_source_y =
        start_anchor_mode_y + (stepper_height + stepper_height) / 2.F +
        intra_section_glue;
    auto constexpr start_anchor_level_y =
        start_anchor_source_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;
    auto constexpr start_anchor_level_cv_y =
        start_anchor_level_y + (small_knob_diameter + port_diameter) / 2.F +
        intra_section_glue;

    auto constexpr end_anchor_mode_y = start_anchor_level_cv_y +
                                       (port_diameter + stepper_height) / 2.F +
                                       inter_section_glue;
    auto constexpr end_anchor_source_y =
        end_anchor_mode_y + (stepper_height + stepper_height) / 2.F +
        intra_section_glue;
    auto constexpr end_anchor_level_y =
        end_anchor_source_y + (stepper_height + small_knob_diameter) / 2.F +
        intra_section_glue;
    auto constexpr end_anchor_level_cv_y =
        end_anchor_level_y + (small_knob_diameter + port_diameter) / 2.F +
        intra_section_glue;

    auto constexpr duration_y = end_anchor_level_cv_y +
                                (port_diameter + small_knob_diameter) / 2.F +
                                inter_section_glue;
    auto constexpr duration_cv_y = duration_y +
                                   (small_knob_diameter + port_diameter) / 2.F +
                                   intra_section_glue;

    auto constexpr shape_y = duration_cv_y +
                             (port_diameter + stepper_height) / 2.F +
                             inter_section_glue;
    auto constexpr curvature_y = shape_y +
                                 (stepper_height + small_knob_diameter) / 2.F +
                                 intra_section_glue;
    auto constexpr curvature_cv_y =
        curvature_y + (small_knob_diameter + port_diameter) / 2.F +
        intra_section_glue;

    for (auto step = 0; step < N; step++) {
      auto const step_left = left + static_cast<float>(step) * step_width;
      auto const step_x = step_left + step_width / 2.F;
      addChild(rack::createLightCentered<ProgressLight>(
          mm2px(step_x, progress_light_y), module,
          Light::Progress + step + step));

      addParam(Toggle::stepper(slug_, "anchor-mode", 2, module, step_x,
                               start_anchor_mode_y,
                               Param::StartAnchorMode + step));
      addParam(Toggle::stepper(slug_, "anchor-source", anchor_source_count,
                               module, step_x, start_anchor_source_y,
                               Param::StartAnchorSource + step));
      addParam(Knob::small(slug_, module, step_x, start_anchor_level_y,
                           Param::StartAnchorLevel + step));
      addInput(Jack::input(slug_, module, step_x, start_anchor_level_cv_y,
                           Input::StartAnchorLevelCV + step));

      addParam(Toggle::stepper(slug_, "anchor-mode", 2, module, step_x,
                               end_anchor_mode_y, Param::EndAnchorMode + step));
      addParam(Toggle::stepper(slug_, "anchor-source", anchor_source_count,
                               module, step_x, end_anchor_source_y,
                               Param::EndAnchorSource + step));
      addParam(Knob::small(slug_, module, step_x, end_anchor_level_y,
                           Param::EndAnchorLevel + step));
      addInput(Jack::input(slug_, module, step_x, end_anchor_level_cv_y,
                           Input::EndAnchorLevelCV + step));

      addParam(Knob::small(slug_, module, step_x, duration_y,
                           Param::Duration + step));
      addInput(Jack::input(slug_, module, step_x, duration_cv_y,
                           Input::DurationCV + step));

      addParam(Toggle::stepper(slug_, "shape", 2, module, step_x, shape_y,
                               Param::Shape + step));
      addParam(Knob::small(slug_, module, step_x, curvature_y,
                           Param::Curvature + step));
      addInput(Jack::input(slug_, module, step_x, curvature_cv_y,
                           Input::CurvatureCV + step));
    }
  }

  void add_global_outputs(float left) {
    auto const x = left + port_radius + padding;

    auto constexpr step_number_y = global_controls_y(0);
    auto constexpr step_phase_y = global_controls_y(1);
    auto constexpr out_y = global_controls_y(4);

    addOutput(
        Jack::output(slug_, module, x, step_number_y, Output::StepNumber));
    addOutput(Jack::output(slug_, module, x, step_phase_y, Output::StepPhase));
    addOutput(Jack::output(slug_, module, x, out_y, Output::Out));
  }

}; // namespace dhe
} // namespace phase_sequencer
} // namespace dhe
