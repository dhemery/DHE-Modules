#pragma once

#include "controls.h"
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
    setSvg(control_svg(module_svg_dir, "marker-start"));
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
    auto const x = step_x + step_dx * static_cast<float>(selection_end);
    this->box.pos.x = mm2px(x);
  }

  int selection_start_{};
  int selection_length_{};
  int const step_mask_ = N - 1;
}; // namespace curve_sequencer

template <int N> class Panel : public rack::app::ModuleWidget {
  using ControlsT =
      curve_sequencer::Controls<rack::engine::Input, rack::engine::Output,
                                rack::engine::Param, rack::engine::Light, N>;

public:
  Panel(rack::engine::Module *module) {
    auto const slug = std::string{"curve-sequencer-"} + std::to_string(N);
    auto constexpr step_width = 2.25F;
    auto constexpr sequence_controls_width = 13.F;
    auto constexpr hp =
        static_cast<int>(sequence_controls_width + N * step_width);

    setModule(module);
    setPanel(background_svg(slug));
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

    addInput(Jack::input(slug, module, left, run_y, ControlsT::Input::Run));
    addParam(Toggle::button(slug, module, left + button_port_distance, run_y,
                            ControlsT::Param::Run));

    addInput(Jack::input(slug, module, left, loop_y, ControlsT::Input::Loop));
    addParam(Toggle::button(slug, module, left + button_port_distance, loop_y,
                            ControlsT::Param::Loop));

    auto *start_marker = new StartMarker(slug, 0.F, active_y);
    addChild(start_marker);

    auto *end_marker = new EndMarker<N>(slug, 0.F, active_y);
    addChild(end_marker);

    auto const on_selection_start_change = [start_marker,
                                            end_marker](int step) {
      start_marker->set_selection_start(step);
      end_marker->set_selection_start(step);
    };
    addParam(new SelectionKnob(on_selection_start_change, slug, module, left,
                               selection_y, ControlsT::Param::SelectionStart));

    auto const on_selection_end_change = [end_marker](int length) {
      end_marker->set_selection_length(length);
    };
    auto constexpr selection_length_x = left + hp2mm(2.F);
    addParam(new SelectionKnob(on_selection_end_change, slug, module,
                               selection_length_x, selection_y,
                               ControlsT::Param::SelectionLength));

    addInput(Jack::input(slug, module, left, gate_y, ControlsT::Input::Gate));
    addParam(Button::momentary(slug, module, left + button_port_distance,
                               gate_y, ControlsT::Param::Gate));

    addInput(Jack::input(slug, module, left, reset_y, ControlsT::Input::Reset));
    addParam(Button::momentary(slug, module, left + button_port_distance,
                               reset_y, ControlsT::Param::Reset));

    auto constexpr level_y = top + hp2mm(6.75F);
    auto constexpr shape_y = top + hp2mm(9.25F);
    auto constexpr curve_y = top + hp2mm(11.75F);
    auto constexpr duration_y = top + hp2mm(14.25F);
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
          mm2px(x, active_y), module,
          ControlsT::Light::StepProgress + step + step));

      addParam(Knob::small(slug, module, x, level_y,
                           ControlsT::Param::StepEndLevel + step));

      addParam(Toggle::stepper(2, slug, module, x, shape_y,
                               ControlsT::Param::StepShape + step));
      addParam(Knob::small(slug, module, x, curve_y,
                           ControlsT::Param::StepCurvature + step));

      addParam(Knob::small(slug, module, x, duration_y,
                           ControlsT::Param::StepDuration + step));

      addParam(Toggle::button(slug, module, x, enabled_button_y,
                              ControlsT::Param::EnableStep + step));
      addInput(Jack::input(slug, module, x, enabled_port_y,
                           ControlsT::Input::EnableStep + step));
    }

    auto constexpr out_y = bottom - port_radius - 1.F;
    auto constexpr eos_y = top + hp2mm(2.75);

    addInput(Jack::input(slug, module, right, eos_y, ControlsT::Input::In));

    addParam(Toggle::stepper(2, slug, module, right, level_y,
                             ControlsT::Param::LevelRange));
    addParam(Toggle::stepper(3, slug, module, right, duration_y,
                             ControlsT::Param::DurationRange));
    addOutput(Jack::output(slug, module, right, out_y, ControlsT::Output::Out));
  }
}; // namespace dhe
} // namespace curve_sequencer
} // namespace dhe
