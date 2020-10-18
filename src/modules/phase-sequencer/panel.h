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

template <int N> class Panel : public rack::app::ModuleWidget {
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;

public:
  Panel(rack::engine::Module *module) {
    auto const slug = std::string{"phase-sequencer-"} + std::to_string(N);

    auto constexpr step_block_width_hp = N * step_width_hp;
    auto constexpr hp = base_width_hp + step_block_width_hp;

    setModule(module);
    setPanel(background_svg(slug));
    install_screws(this, static_cast<int>(hp));

    auto constexpr content_width = sequence_controls_width +
                                   global_controls_width + labels_width +
                                   hp2mm(step_block_width_hp);
    auto constexpr excess_width = hp2mm(hp) - padding - content_width;
    auto constexpr margin = excess_width / 4.F;

    auto constexpr sequence_controls_left = margin;
    auto constexpr global_controls_left =
        sequence_controls_left + sequence_controls_width + margin;
    auto constexpr step_block_left = global_controls_left +
                                     global_controls_width + margin +
                                     labels_width + padding;

    auto constexpr progress_light_y = top - light_diameter * 2.F;

    auto constexpr level_y = global_controls_y(0);
    auto constexpr in_y = global_controls_y(2);
    auto constexpr state_y = global_controls_y(3);
    auto constexpr out_y = global_controls_y(4);

    auto constexpr global_controls_left_x =
        global_controls_left + global_control_width / 2.F;
    auto constexpr global_controls_center_x =
        global_controls_left_x + global_control_width + padding;
    auto constexpr global_controls_right_x =
        global_controls_center_x + global_control_width + padding;

    addParam(Toggle::thumb(2, slug, module, global_controls_right_x, level_y,
                           Param::LevelRange));

    addInput(
        Jack::input(slug, module, global_controls_left_x, in_y, Input::InA));
    addInput(
        Jack::input(slug, module, global_controls_center_x, in_y, Input::InB));
    addInput(
        Jack::input(slug, module, global_controls_right_x, in_y, Input::InC));

    addOutput(Jack::output(slug, module, global_controls_left_x, state_y,
                           Output::StepNumber));

    addOutput(Jack::output(slug, module, global_controls_right_x, out_y,
                           Output::Out));

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

    for (auto step = 0; step < N; step++) {
      auto const step_left =
          step_block_left + static_cast<float>(step) * step_width;
      auto const step_x = step_left + step_width / 2.F;
      addChild(rack::createLightCentered<ProgressLight>(
          mm2px(step_x, progress_light_y), module,
          Light::Progress + step + step));

      addParam(Toggle::stepper(slug, "anchor-mode", 2, module, step_x,
                               start_anchor_mode_y,
                               Param::StartAnchorMode + step));
      addParam(Knob::small(slug, module, step_x, start_anchor_level_y,
                           Param::StartAnchorLevel + step));
      addParam(Toggle::stepper(slug, "anchor-source", anchor_source_count,
                               module, step_x, start_anchor_source_y,
                               Param::StartAnchorSource + step));

      addParam(Toggle::stepper(slug, "anchor-mode", 2, module, step_x,
                               end_anchor_mode_y, Param::EndAnchorMode + step));
      addParam(Knob::small(slug, module, step_x, end_anchor_level_y,
                           Param::EndAnchorLevel + step));
      addParam(Toggle::stepper(slug, "anchor-source", anchor_source_count,
                               module, step_x, end_anchor_source_y,
                               Param::EndAnchorSource + step));

      addParam(Knob::small(slug, module, step_x, duration_y,
                           Param::Duration + step));

      addParam(Toggle::stepper(slug, "shape", 2, module, step_x, shape_y,
                               Param::Shape + step));
      addParam(Knob::small(slug, module, step_x, curvature_y,
                           Param::Curvature + step));
    }
  }

private:
}; // namespace dhe
} // namespace phase_sequencer
} // namespace dhe
