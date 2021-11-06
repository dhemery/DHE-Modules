#pragma once

#include "control-ids.h"
#include "engine.h"
#include "generate-mode.h"
#include "params.h"
#include "signals.h"
#include "step-controller.h"
#include "step-selector.h"

#include "components/phase-timer.h"
#include "controls/levels.h"
#include "controls/scaled-quantity.h"
#include "controls/switches.h"
#include "params/curvature-config.h"
#include "params/duration-config.h"
#include "params/level-config.h"
#include "params/presets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

namespace curve_sequencer {
template <int N> struct Module : public rack::engine::Module {
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
  using Light = LightIds<N>;

  Module() {
    config(Param::Count, Input::Count, Output::Count, Light::Count);

    Button::config(this, Param::Run, "Run", 1);
    configInput(Input::Run, "Run");
    Button::config(this, Param::Gate, "Gate");
    configInput(Input::Gate, "Gate");
    Button::config(this, Param::Loop, "Loop");
    configInput(Input::Loop, "Loop");
    Button::config(this, Param::Reset, "Reset");
    configInput(Input::Reset, "Reset");

    auto const selection_start_knob =
        configParam(Param::SelectionStart, 0.F, N - 1, 0.F, "Start step", "",
                    0.F, 1.F, 1.F);
    selection_start_knob->snapEnabled = true;
    auto const selection_length_knob = configParam(
        Param::SelectionLength, 1.F, N, N, "Sequence length", " steps");
    selection_length_knob->snapEnabled = true;

    auto level_knobs = std::vector<ScaledQuantity *>{};

    for (auto step = 0; step < N; step++) {
      auto const step_name =
          std::string{"Step "} + std::to_string(step + 1) + " ";
      configLight(Light::StepProgress + step + step, step_name + "progress");
      Stepper<GenerateModes>::config(this, Param::StepGenerateMode + step,
                                     step_name + "generate mode",
                                     GenerateMode::Curve);
      Stepper<AdvanceModes>::config(this, Param::StepAdvanceMode + step,
                                    step_name + "advance mode",
                                    AdvanceMode::TimerExpires);
      auto *level_knob = Knob::config<Unipolar>(this, Param::StepLevel + step,
                                                step_name + "level", 5.F);
      level_knobs.push_back(level_knob);

      config_curve_shape_switch(this, Param::StepShape + step,
                                step_name + "shape");
      config_curvature_knob(this, Param::StepCurvature + step,
                            step_name + "curvature");
      config_duration_knob(this, Param::StepDuration + step,
                           Param::DurationRange, step_name + "duration");
      Button::config(this, Param::StepEnabled + step, step_name + "enabled", 1);
      configInput(Input::StepEnabled + step, step_name + "enabled");

      signals_.show_inactive(step);
    }

    auto update_level_knob_ranges = [level_knobs](Range r) {
      for (auto *level_knob : level_knobs) {
        level_knob->set_range(r);
      }
    };

    ItemSwitch::config<Levels>(this, Param::LevelRange, "Level range",
                               Levels::Unipolar)
        ->on_change(update_level_knob_ranges);

    config_duration_range_switch(this, Param::DurationRange);

    configInput(Input::Main, "AUX");
    configOutput(Output::Main, "Sequencer");
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    engine_.execute(args.sampleTime);
  }

private:
  using RackSignals = Signals<rack::engine::Param, rack::engine::Input,
                              rack::engine::Output, rack::engine::Light, N>;
  PhaseTimer timer_{};
  RackSignals signals_{params, inputs, outputs, lights};
  StepController<RackSignals> step_controller_{signals_, timer_};
  StepSelector<RackSignals> selector_{signals_, N};

  Engine<RackSignals, StepSelector<RackSignals>, StepController<RackSignals>>
      engine_{signals_, selector_, step_controller_};
};
} // namespace curve_sequencer

} // namespace dhe
