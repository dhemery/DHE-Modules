#pragma once

#include "control-ids.h"
#include "engine.h"
#include "generate-mode.h"
#include "params.h"
#include "signals.h"
#include "step-controller.h"
#include "step-selector.h"

#include "components/phase-timer.h"
#include "controls/buttons.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/curvature.h"
#include "signals/durations.h"
#include "signals/step-selection.h"
#include "signals/voltages.h"

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

    IntKnob::config<SelectionStart<N>>(this, Param::SelectionStart,
                                       "Start step", 0);
    IntKnob::config<SelectionLength<N>>(this, Param::SelectionLength,
                                        "Sequence length", N);

    auto level_knobs = std::vector<MappedKnobQuantity<Voltages> *>{};
    auto duration_knobs = std::vector<MappedKnobQuantity<Durations> *>{};

    for (auto step = 0; step < N; step++) {
      auto const step_name =
          std::string{"Step "} + std::to_string(step + 1) + " ";
      configLight(Light::StepProgress + step + step, step_name + "progress");
      Switch::config<GenerateModes>(this, Param::StepGenerateMode + step,
                                    step_name + "generate mode",
                                    GenerateMode::Curve);
      Switch::config<AdvanceModes>(this, Param::StepAdvanceMode + step,
                                   step_name + "advance mode",
                                   AdvanceMode::TimerExpires);
      auto *level_knob = Knob::config<Voltages>(this, Param::StepLevel + step,
                                                step_name + "level");
      level_knobs.push_back(level_knob);

      Switch::config<Shapes>(this, Param::StepShape + step, step_name + "shape",
                             Shapes::J);
      Knob::config<Curvature>(this, Param::StepCurvature + step,
                              step_name + "curvature");
      auto *duration_knob = Knob::config<Durations>(
          this, Param::StepDuration + step, step_name + "duration");
      duration_knobs.push_back(duration_knob);

      Button::config(this, Param::StepEnabled + step, step_name + "enabled", 1);
      configInput(Input::StepEnabled + step, step_name + "enabled");

      signals_.show_inactive(step);
    }

    auto select_level_range = [level_knobs](Voltages::ValueType selection) {
      for (auto *knob : level_knobs) {
        knob->mapper().select_range(selection);
      }
    };
    Switch::config<Voltages>(this, Param::LevelRange, "Level range",
                             Voltages::Unipolar)
        ->on_change(select_level_range);

    auto select_duration_range =
        [duration_knobs](Durations::ValueType selection) {
          for (auto *knob : duration_knobs) {
            knob->mapper().select_range(selection);
          }
        };
    Switch::config<Durations>(this, Param::DurationRange, "Duration range",
                              Durations::Medium)
        ->on_change(select_duration_range);

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
