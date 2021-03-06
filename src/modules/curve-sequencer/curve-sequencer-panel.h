#pragma once

#include "curve-sequencer-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>
#include <componentlibrary.hpp>
#include <functional>
#include <string>
#include <utility>

namespace dhe {

namespace curve_sequencer {
auto constexpr step_x = hp2mm(10.F);
auto constexpr step_dx = hp2mm(2.25F);

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
    setSvg(load_svg(module_svg_dir, "marker-start"));
    position_centered(this, x, y);
  }

  void set_selection_start(int step) {
    auto constexpr base_x = step_x - 2.F * light_diameter;
    auto const x = base_x + step_dx * static_cast<float>(step);
    this->box.pos.x = mm2px(x);
  }
};

template <int N> class EndMarker : public rack::widget::SvgWidget {
public:
  EndMarker(std::string const &module_svg_dir, float x, float y) {
    setSvg(load_svg(module_svg_dir, "marker-end"));
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
    auto const x = step_x + step_dx * static_cast<float>(selection_end);
    this->box.pos.x = mm2px(x);
  }

  int selection_start_{};
  int selection_length_{};
  int const step_mask_ = N - 1;
}; // namespace curve_sequencer

template <int N> class CurveSequencerPanel : public rack::app::ModuleWidget {
  using Controls =
      CurveSequencerControls<rack::engine::Input, rack::engine::Output,
                             rack::engine::Param, rack::engine::Light, N>;

public:
  CurveSequencerPanel(rack::engine::Module *module) {
    auto const svg_dir = "curve-sequencer";
    auto constexpr step_width = 2.25F;
    auto constexpr sequence_controls_width = 13.F;
    auto constexpr hp =
        static_cast<int>(sequence_controls_width + N * step_width);

    setModule(module);
    setPanel(
        load_svg(svg_dir, std::string{"curve-sequencer-"} + std::to_string(N)));
    install_screws(this, hp);

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

    addInput(Jack::input(svg_dir, module, left, run_y, Controls::RunInput));
    addParam(Button::toggle(svg_dir, module, left + button_port_distance, run_y,
                            Controls::RunButton));

    addInput(Jack::input(svg_dir, module, left, loop_y, Controls::LoopInput));
    addParam(Button::toggle(svg_dir, module, left + button_port_distance,
                            loop_y, Controls::LoopButton));

    auto *start_marker = new StartMarker(svg_dir, 0.F, active_y);
    addChild(start_marker);

    auto *end_marker = new EndMarker<N>(svg_dir, 0.F, active_y);
    addChild(end_marker);

    auto const on_selection_start_change = [start_marker,
                                            end_marker](int step) {
      start_marker->set_selection_start(step);
      end_marker->set_selection_start(step);
    };
    addParam(new SelectionKnob(on_selection_start_change, svg_dir, module, left,
                               selection_y, Controls::SelectionStartKnob));

    auto const on_selection_end_change = [end_marker](int length) {
      end_marker->set_selection_length(length);
    };
    auto constexpr selection_length_x = left + hp2mm(2.F);
    addParam(new SelectionKnob(on_selection_end_change, svg_dir, module,
                               selection_length_x, selection_y,
                               Controls::SelectionLengthKnob));

    addInput(Jack::input(svg_dir, module, left, gate_y, Controls::GateInput));
    addParam(Button::momentary(svg_dir, module, left + button_port_distance,
                               gate_y, Controls::GateButton));

    addInput(Jack::input(svg_dir, module, left, reset_y, Controls::ResetInput));
    addParam(Button::momentary(svg_dir, module, left + button_port_distance,
                               reset_y, Controls::ResetButton));

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
      addChild(rack::createLightCentered<ProgressLight>(
          mm2px(x, active_y), module, Controls::ProgressLights + step + step));

      auto *generate_mode_button = Toggle::stepper(
          svg_dir, "generate-mode", generate_mode_labels.size(), module, x,
          generate_mode_y, Controls::ModeSwitches + step);
      addParam(generate_mode_button);

      auto *advance_mode_button = Toggle::stepper(
          svg_dir, "advance-mode", advance_mode_labels.size(), module, x,
          advance_mode_y, Controls::ConditionSwitches + step);
      addParam(advance_mode_button);

      addParam(Knob::small(svg_dir, module, x, level_y,
                           Controls::LevelKnobs + step));

      addParam(Toggle::thumb(2, svg_dir, module, x, shape_y,
                             Controls::ShapeSwitches + step));
      addParam(Knob::small(svg_dir, module, x, curve_y,
                           Controls::CurveKnobs + step));

      addParam(Knob::small(svg_dir, module, x, duration_y,
                           Controls::DurationKnobs + step));

      addParam(Button::toggle(svg_dir, module, x, enabled_button_y,
                              Controls::EnabledButtons + step));
      addInput(Jack::input(svg_dir, module, x, enabled_port_y,
                           Controls::EnabledInputs + step));
    }

    auto constexpr out_y = bottom - port_radius - 1.F;
    auto constexpr eos_y = top + hp2mm(2.75);

    addInput(Jack::input(svg_dir, module, right, eos_y,
                         Controls::CurveSequencerInput));

    addParam(Toggle::thumb(2, svg_dir, module, right, level_y,
                           Controls::LevelRangeSwitch));
    addParam(Toggle::thumb(3, svg_dir, module, right, duration_y,
                           Controls::DurationRangeSwitch));
    addOutput(Jack::output(svg_dir, module, right, out_y,
                           Controls::CurveSequencerOutput));
  }
}; // namespace dhe
} // namespace curve_sequencer
} // namespace dhe
