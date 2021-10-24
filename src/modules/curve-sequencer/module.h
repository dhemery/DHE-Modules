#pragma once

#include "control-ids.h"
#include "engine.h"
#include "generate-mode.h"
#include "params.h"
#include "signals.h"
#include "step-controller.h"
#include "step-selector.h"

#include "components/phase-timer.h"
#include "controls/steppers.h"
#include "params/curvature-config.h"
#include "params/duration-config.h"
#include "params/level-config.h"
#include "params/presets.h"

#include "rack.hpp"

namespace dhe {

namespace curve_sequencer {
template <int N> struct Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Param = ParamIds<N>;
  using Output = OutputIds;

  Module() {
    config(Param::Count, Input::InputCount, Output::OutputCount,
           Light::LightCount);

    Button::config(this, Param::Run, "Run", 1);
    configInput(Input::RunInput, "Run");
    Button::config(this, Param::Gate, "Gate");
    configInput(Input::GateInput, "Gate");
    Button::config(this, Param::Loop, "Loop");
    configInput(Input::LoopInput, "Loop");
    Button::config(this, Param::Reset, "Reset");
    configInput(Input::ResetInput, "Reset");

    auto const selection_start_knob =
        configParam(Param::SelectionStart, 0.F, N - 1, 0.F, "Start step", "",
                    0.F, 1.F, 1.F);
    selection_start_knob->snapEnabled = true;
    auto const selection_length_knob = configParam(
        Param::SelectionLength, 1.F, N, N, "Sequence length", " steps");
    selection_length_knob->snapEnabled = true;

    config_level_range_switch(this, Param::LevelRange);
    config_duration_range_switch(this, Param::DurationRange);

    for (auto step = 0; step < N; step++) {
      auto const step_name =
          std::string{"Step "} + std::to_string(step + 1) + " ";
      configLight(Light::ProgressLights + step + step, step_name + "progress");
      Stepper<GenerateModes>::config(this, Param::GenerateMode + step,
                                     step_name + "generate mode",
                                     GenerateMode::Curve);
      Stepper<AdvanceModes>::config(this, Param::AdvanceMode + step,
                                    step_name + "advance mode",
                                    AdvanceMode::TimerExpires);
      config_level_knob(this, Param::Level + step, Param::LevelRange,
                        step_name + "level");
      config_curve_shape_switch(this, Param::Shape + step, step_name + "shape");
      config_curvature_knob(this, Param::Curvature + step,
                            step_name + "curvature");
      config_duration_knob(this, Param::Duration + step, Param::DurationRange,
                           step_name + "duration");
      Button::config(this, Param::Enabled + step, step_name + "enabled", 1);
      configInput(Input::EnabledInputs + step, step_name + "enabled");

      signals_.show_inactive(step);
    }

    configInput(Input::CurveSequencerInput, "AUX");
    configOutput(Output::CurveSequencerOutput, "Sequencer");
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
