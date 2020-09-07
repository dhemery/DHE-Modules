#pragma once

#include "advancement.h"
#include "anchor.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "controls.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "generator.h"
#include "sequence-controller.h"
#include "step-controller.h"
#include "step-selector.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curve_sequencer {
// Skew the progress::brightness ratio so that the "remaining" light stays
// fully lit for a little while during early progress, and the "completed"
// light reaches fully lit a little while before progress is complete.
static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <int N> class Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;

public:
  Module() {
    config(Param::Count, Input::Count, Output::Count, Light::Count);

    config_button(this, Param::Run, "Run", {"From input", "Yes"}, 1);
    config_button(this, Param::Gate, "Gate", {"From input", "High"}, 0);
    config_button(this, Param::Loop, "Loop", {"From input", "Yes"}, 0);
    config_button(this, Param::Reset, "Reset", {"From input", "High"}, 0);

    configParam(Param::SelectionStart, 0.F, N - 1, 0.F, "Start step", "", 0.F,
                1.F, 1.F);
    configParam(Param::SelectionLength, 1.F, N, N, "Sequence length", " steps");

    config_level_range_switch(this, Param::LevelRange);
    config_duration_range_switch(this, Param::DurationRange);

    for (auto step = 0; step < N; step++) {
      config_toggle<trigger_mode_count>(this, Param::StepTriggerMode + step,
                                        "Trigger mode",
                                        trigger_mode_descriptions, 0);
      config_toggle<2>(
          this, Param::StepInterruptMode + step, "Interrupt",
          {"Ignore triggers while generating", "Advance to next step"});
      config_toggle<2>(this, Param::StepCompletionMode + step, "At end",
                       {"Sustain until triggered", "Advance to next step"}, 1);

      config_toggle<4>(this, Param::StepStartAnchorSource + step,
                       "Start anchor source",
                       {"Level knob", "In port", "Out port", "Aux port"}, 2);
      config_level_knob(this, Param::StepStartLevel + step, Param::LevelRange,
                        "Start level");
      config_toggle<2>(this, Param::StepStartAnchorMode + step,
                       "Start anchor mode", {"Snapshot", "Track changes"});

      config_toggle<4>(this, Param::StepEndAnchorSource + step,
                       "End anchor source",
                       {"Level knob", "In port", "Out port, Aux port"});
      config_level_knob(this, Param::StepEndLevel + step, Param::LevelRange,
                        "End level");
      config_toggle<2>(this, Param::StepEndAnchorMode + step, "End anchor mode",
                       {"Snapshot", "Track changes"}, 1);

      config_curve_shape_switch(this, Param::StepShape + step, "Shape");
      config_curvature_knob(this, Param::StepCurvature + step, "Curvature");
      config_duration_knob(this, Param::StepDuration + step,
                           Param::DurationRange, "Duration");
      config_button(this, Param::EnableStep + step, "Enabled",
                    {"From input", "Yes"}, 1);

      show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    sequence_controller_.execute(args.sampleTime);
  }

  auto anchor_level(AnchorType type, int step) const -> float {
    auto const base =
        type == AnchorType::Start ? Param::StepStartLevel : Param::StepEndLevel;
    return dhe::selectable_level(params[base + step],
                                 params[Param::LevelRange]);
  }

  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    auto const base = type == AnchorType::Start ? Param::StepStartAnchorMode
                                                : Param::StepEndAnchorMode;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorMode>(selection);
  }

  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    auto const base = type == AnchorType::Start ? Param::StepStartAnchorSource
                                                : Param::StepEndAnchorSource;
    auto const selection = position_of(params[base + step]);
    return static_cast<AnchorSource>(selection);
  }

  auto aux() const -> float { return voltage_at(inputs[Input::Aux]); }

  auto completion_mode(int step) const -> CompletionMode {
    auto const selection =
        position_of(params[Param::StepCompletionMode + step]);
    return static_cast<CompletionMode>(selection);
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params[Param::StepCurvature + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params[Param::StepDuration + step],
                                    inputs[Input::DurationCV],
                                    params[Param::DurationRange]);
  }

  auto gate() const -> bool {
    return is_high(inputs[Input::Gate]) || is_pressed(params[Param::Gate]);
  }

  auto input() const -> float { return voltage_at(inputs[Input::In]); }

  auto interrupt_mode(int step) const -> InterruptMode {
    auto const selection = position_of(params[Param::StepInterruptMode + step]);
    return static_cast<InterruptMode>(selection);
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params[Param::EnableStep + step]) ||
           is_high(inputs[Input::EnableStep + step]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params[Param::Loop]) || is_high(inputs[Input::Loop]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs[Input::Reset]) || is_pressed(params[Param::Reset]);
  }

  auto is_running() const -> bool {
    return is_pressed(params[Param::Run]) || is_high(inputs[Input::Run]);
  }

  auto output() const -> float { return voltage_at(outputs[Output::Out]); }

  void output(float voltage) { outputs[Output::Out].setVoltage(voltage); }

  auto selection_start() const -> int {
    return static_cast<int>(rotation_of(params[Param::SelectionStart]));
  }

  auto selection_length() const -> int {
    return static_cast<int>(rotation_of(params[Param::SelectionLength]));
  }

  auto trigger_mode(int step) const -> TriggerMode {
    auto const selection = position_of(params[Param::StepTriggerMode + step]);
    return static_cast<TriggerMode>(selection);
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection = position_of(params[Param::StepShape + step]);
    return sigmoid::tapers[selection];
  }

private:
  using AnchorT = Anchor<Module<N>>;
  using GeneratorT = Generator<Module<N>, AnchorT>;
  using InterrupterT = Interrupter<Module<N>>;
  using StepSelectorT = StepSelector<Module<N>>;
  using SustainerT = Sustainer<Module<N>>;

  AnchorT end_anchor_{*this, AnchorType::End};
  AnchorT start_anchor_{*this, AnchorType::Start};
  GeneratorT generator_{*this, start_anchor_, end_anchor_};
  InterrupterT interrupter_{*this};
  StepSelectorT step_selector_{*this, N};
  SustainerT sustainer_{*this};

  using StepControllerT = StepController<InterrupterT, GeneratorT, SustainerT>;
  using SequenceControllerT =
      SequenceController<Module<N>, StepSelectorT, StepControllerT>;

  StepControllerT step_controller_{interrupter_, generator_, sustainer_};
  SequenceControllerT sequence_controller_{*this, step_selector_,
                                           step_controller_};

  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = Light::StepProgress + step + step;
    auto const remaining_light = completed_light + 1;
    lights[completed_light].setBrightness(completed_brightness);
    lights[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace curve_sequencer

} // namespace dhe
