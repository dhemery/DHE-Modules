#pragma once

#include "controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"
#include <app/ModuleWidget.hpp>
#include <cmath>
#include <componentlibrary.hpp>
#include <functional>
#include <jansson.h>
#include <string>
#include <utility>

namespace dhe {

namespace cv_sequencer {
static auto constexpr step_width = hp2mm(2.5F);
static auto constexpr margin = 6.F;
static auto constexpr top = 23.F;
static auto constexpr bottom = 117.F;
static auto constexpr x_offset = 4.5847F;
static auto constexpr label_small_size = 7.0F / px_per_mm;
static auto constexpr small_knob_diameter = 8.4F;

static auto constexpr sequence_controls_left = 0.F;
static auto constexpr sequence_controls_width =
    padding + port_diameter + padding + button_diameter + padding;
static auto constexpr sequence_controls_right =
    sequence_controls_left + sequence_controls_width;

static auto constexpr global_step_controls_column_width =
    padding + port_diameter + padding;
static auto constexpr global_step_controls_left =
    sequence_controls_right + margin;
static auto constexpr global_step_controls_width =
    global_step_controls_column_width + padding +
    global_step_controls_column_width + padding +
    global_step_controls_column_width;
static auto constexpr global_step_controls_right =
    global_step_controls_left + global_step_controls_width;

static auto constexpr labels_left = global_step_controls_right + margin;
static auto constexpr labels_width = 0.67F * step_width;
static auto constexpr labels_right = labels_left + labels_width;

static auto constexpr step_block_left = labels_right + padding;
static auto constexpr port_box_ascent =
    padding + label_small_size + port_radius + 0.22F;
static auto constexpr port_box_descent = padding + port_radius;
static auto constexpr global_controls_top_y = top + port_box_ascent;
static auto constexpr global_controls_bottom_y = bottom - port_box_descent;
static auto constexpr global_controls_dy =
    (global_controls_bottom_y - global_controls_top_y) / 4.F;

static auto constexpr step_block_x = step_block_left + x_offset;

static inline auto constexpr global_controls_y(int row) -> float {
  return global_controls_top_y + static_cast<float>(row) * global_controls_dy;
}

using ProgressLight =
    rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>;

class SelectionKnob : public Knob {
public:
  SelectionKnob(std::function<void(int)> action,
                std::string const &module_svg_dir, rack::engine::Module *module,
                float x, float y, int index)
      : Knob{module_svg_dir, "knob-small", module, x, y, index},
        knob_changed_to_{std::move(action)} {
    snap = true;
  }

  void onChange(const rack::event::Change &e) override {
    Knob::onChange(e);
    knob_changed_to_(static_cast<int>(this->paramQuantity->getValue()));
  }

private:
  std::function<void(int)> knob_changed_to_;
};

class StartMarker : public rack::widget::SvgWidget {
public:
  StartMarker(std::string const &module_svg_dir, float x, float y) {
    setSvg(control_svg(module_svg_dir, "marker-start"));
    position_centered(this, x, y);
  }

  void set_selection_start(int step) {
    auto const x = step_block_x + step_width * static_cast<float>(step) +
                   step_width / 2.F - light_diameter * 2.F;
    this->box.pos.x = mm2px(x);
  }
};

template <int N> class EndMarker : public rack::widget::SvgWidget {
public:
  EndMarker(std::string const &module_svg_dir, float x, float y) {
    setSvg(control_svg(module_svg_dir, "marker-end"));
    position_centered(this, x, y);
  }

  void set_selection_start(int step) {
    this->selection_start_ = step;
    move();
  }
  void set_selection_length(int length) {
    this->selection_length_ = length;
    move();
  }

private:
  void move() {
    auto const selection_end =
        (selection_start_ + selection_length_ - 1) & step_mask_;
    auto const x = step_block_x +
                   step_width * static_cast<float>(selection_end) +
                   step_width / 2.F;
    this->box.pos.x = mm2px(x);
  }

  int selection_start_{};
  int selection_length_{};
  int const step_mask_ = N - 1;
}; // namespace cv_sequencer

template <int N> class Panel : public rack::app::ModuleWidget {
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;

public:
  Panel(rack::engine::Module *module) {
    auto const slug = std::string{"sequence-foundry-"} + std::to_string(N);

    auto constexpr content_width = 14.195 + static_cast<float>(N) * step_width;

    auto constexpr content_width_hp = mm2hp(content_width);
    auto constexpr hp = static_cast<int>(content_width_hp + 2.F);

    setModule(module);
    setPanel(background_svg(slug));
    install_screws(this, hp);

    auto constexpr sequence_controls_x =
        sequence_controls_left + port_radius + padding + x_offset;

    auto constexpr run_y = global_controls_y(0);
    addInput(Jack::input(slug, module, sequence_controls_x, run_y, Input::Run));
    addParam(Button::toggle(slug, module,
                            sequence_controls_x + button_port_distance, run_y,
                            Param::Run));

    auto constexpr loop_y = global_controls_y(1);
    addInput(
        Jack::input(slug, module, sequence_controls_x, loop_y, Input::Loop));
    addParam(Button::toggle(slug, module,
                            sequence_controls_x + button_port_distance, loop_y,
                            Param::Loop));

    auto constexpr progress_light_y = top - light_diameter * 2.F;
    auto *start_marker = new StartMarker(slug, 0.F, progress_light_y);
    addChild(start_marker);

    auto *end_marker = new EndMarker<N>(slug, 0.F, progress_light_y);
    addChild(end_marker);

    auto const on_selection_start_change = [start_marker,
                                            end_marker](int step) {
      start_marker->set_selection_start(step);
      end_marker->set_selection_start(step);
    };

    auto constexpr selection_y = global_controls_y(2);
    addParam(new SelectionKnob(on_selection_start_change, slug, module,
                               sequence_controls_x - hp2mm(0.2F), selection_y,
                               Param::SelectionStart));

    auto const on_selection_end_change = [end_marker](int length) {
      end_marker->set_selection_length(length);
    };
    auto constexpr selection_length_offset = 8.28F;
    auto constexpr selection_length_x =
        sequence_controls_x + selection_length_offset;
    addParam(new SelectionKnob(on_selection_end_change, slug, module,
                               selection_length_x, selection_y,
                               Param::SelectionLength));

    auto constexpr gate_y = global_controls_y(3);
    addInput(
        Jack::input(slug, module, sequence_controls_x, gate_y, Input::Gate));
    addParam(Button::momentary(slug, module,
                               sequence_controls_x + button_port_distance,
                               gate_y, Param::Gate));

    auto constexpr reset_y = global_controls_y(4);
    addInput(
        Jack::input(slug, module, sequence_controls_x, reset_y, Input::Reset));
    addParam(Button::momentary(slug, module,
                               sequence_controls_x + button_port_distance,
                               reset_y, Param::Reset));

    auto constexpr level_y = global_controls_y(0);
    auto constexpr global_duration_y = global_controls_y(1);
    auto constexpr in_y = global_controls_y(2);
    auto constexpr state_y = global_controls_y(3);
    auto constexpr out_y = global_controls_y(4);

    auto constexpr global_controls_left_x =
        global_step_controls_left + global_step_controls_column_width / 2.F +
        x_offset;
    auto constexpr global_controls_center_x =
        global_controls_left_x + global_step_controls_column_width + padding;
    auto constexpr global_controls_right_x =
        global_controls_center_x + global_step_controls_column_width + padding;

    addParam(Knob::small(slug, module, global_controls_left_x, level_y,
                         Param::GlobalLevel));
    addInput(Jack::input(slug, module, global_controls_center_x, level_y,
                         Input::GlobalLevelCV));
    addParam(Toggle::thumb(2, slug, module, global_controls_right_x, level_y,
                           Param::GlobalLevelRange));

    addParam(Knob::small(slug, module, global_controls_left_x,
                         global_duration_y, Param::GlobalDuration));
    addInput(Jack::input(slug, module, global_controls_center_x,
                         global_duration_y, Input::GlobalDurationCV));
    addParam(Toggle::thumb(3, slug, module, global_controls_right_x,
                           global_duration_y, Param::GlobalDurationRange));

    addInput(
        Jack::input(slug, module, global_controls_left_x, in_y, Input::InA));
    addInput(
        Jack::input(slug, module, global_controls_center_x, in_y, Input::InB));
    addInput(Jack::input(slug, module, global_controls_right_x, in_y,
                         Input::Trigger));

    addOutput(Jack::output(slug, module, global_controls_left_x, state_y,
                           Output::StepNumber));
    addOutput(Jack::output(slug, module, global_controls_center_x, state_y,
                           Output::IsCurving));
    addOutput(Jack::output(slug, module, global_controls_right_x, state_y,
                           Output::IsSustaining));

    addOutput(Jack::output(slug, module, global_controls_left_x, out_y,
                           Output::StepEvent));
    addOutput(Jack::output(slug, module, global_controls_center_x, out_y,
                           Output::SequenceEvent));
    addOutput(Jack::output(slug, module, global_controls_right_x, out_y,
                           Output::Out));

    auto constexpr intra_section_glue = 0.5F;
    auto constexpr inter_section_glue = 4.F;
    auto constexpr stepper_ascent = label_small_size / 2.F + padding - 0.25F;
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
          step_block_x + static_cast<float>(step) * step_width;
      auto const step_x = step_left + step_width / 2.F;
      addChild(rack::createLightCentered<ProgressLight>(
          mm2px(step_x, progress_light_y), module,
          Light::StepProgress + step + step));

      addParam(Toggle::stepper(slug, "trigger-mode", trigger_mode_count, module,
                               step_x, trigger_y,
                               Param::StepTriggerMode + step));
      addParam(Toggle::stepper(slug, "interrupt-mode", 2, module, step_x,
                               interrupt_y, Param::StepInterruptMode + step));
      addParam(Toggle::stepper(slug, "completion-mode", 2, module, step_x,
                               sustain_y, Param::StepCompletionMode + step));

      addParam(Toggle::stepper(slug, "anchor-mode", 2, module, step_x,
                               start_anchor_mode_y,
                               Param::StepStartAnchorMode + step));
      addParam(Knob::small(slug, module, step_x, start_anchor_level_y,
                           Param::StepStartAnchorLevel + step));
      addParam(Toggle::stepper(slug, "anchor-source", anchor_source_count,
                               module, step_x, start_anchor_source_y,
                               Param::StepStartAnchorSource + step));

      addParam(Toggle::stepper(slug, "anchor-mode", 2, module, step_x,
                               end_anchor_mode_y,
                               Param::StepEndAnchorMode + step));
      addParam(Knob::small(slug, module, step_x, end_anchor_level_y,
                           Param::StepEndAnchorLevel + step));
      addParam(Toggle::stepper(slug, "anchor-source", anchor_source_count,
                               module, step_x, end_anchor_source_y,
                               Param::StepEndAnchorSource + step));

      addParam(Knob::small(slug, module, step_x, duration_y,
                           Param::StepDuration + step));

      addParam(Toggle::stepper(slug, "shape", 2, module, step_x, shape_y,
                               Param::StepShape + step));
      addParam(Knob::small(slug, module, step_x, curvature_y,
                           Param::StepCurvature + step));

      addParam(Button::toggle(slug, module, step_x, enabled_y,
                              Param::StepEnabled + step));
    }
  }

private:
}; // namespace dhe
} // namespace cv_sequencer
} // namespace dhe
