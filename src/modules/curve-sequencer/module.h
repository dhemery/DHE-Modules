#pragma once

#include "components/phase-timer.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "controls.h"
#include "generator.h"
#include "sequence-controller.h"
#include "step-controller.h"
#include "step-selector.h"

#include <engine/Module.hpp>

namespace dhe {

namespace curve_sequencer {
template <int N> class Module : public rack::engine::Module {
  using ControlsT = Controls<rack::engine::Input, rack::engine::Output,
                             rack::engine::Param, rack::engine::Light, N>;

public:
  Module() {
    config(ControlsT::Param::Count, ControlsT::Input::Count,
           ControlsT::Output::Count, ControlsT::Light::Count);

    config_button(this, ControlsT::Param::Run, "Run", {"From input", "Yes"}, 1);
    config_button(this, ControlsT::Param::Gate, "Gate", {"From input", "High"},
                  0);
    config_button(this, ControlsT::Param::Loop, "Loop", {"From input", "Yes"},
                  0);
    config_button(this, ControlsT::Param::Reset, "Reset",
                  {"From input", "High"}, 0);

    configParam(ControlsT::Param::SelectionStart, 0.F, N - 1, 0.F, "Start step",
                "", 0.F, 1.F, 1.F);
    configParam(ControlsT::Param::SelectionLength, 1.F, N, N, "Sequence length",
                " steps");

    config_level_range_switch(this, ControlsT::Param::LevelRange);
    config_duration_range_switch(this, ControlsT::Param::DurationRange);

    for (auto step = 0; step < N; step++) {
      config_toggle<trigger_mode_count>(
          this, ControlsT::Param::StepTriggerMode + step,
          "Generate trigger when", trigger_mode_descriptions, 0);
      config_toggle<2>(this, ControlsT::Param::InterruptStepOnTrigger + step,
                       "When triggered",
                       {"Continue current step", "Advance to next step"});
      config_toggle<2>(this, ControlsT::Param::AdvanceStepOnEndOfCurve + step,
                       "When curve ends",
                       {"Sustain current step", "Advance to next step"}, 1);

      config_toggle<3>(
          this, ControlsT::Param::StepStartSource + step, "Curve from",
          {"Knob level", "Voltage at IN port", "Voltage at OUT port"}, 2);
      config_level_knob(this, ControlsT::Param::StepStartLevel + step,
                        ControlsT::Param::LevelRange, "Start level");
      config_toggle<2>(this, ControlsT::Param::StepTracksStartSource + step,
                       "While generating", {"Ignore changes", "Track changes"});

      config_toggle<3>(
          this, ControlsT::Param::StepEndSource + step, "Curve to",
          {"Knob level", "Voltage at IN port", "Voltage at OUT port"});
      config_level_knob(this, ControlsT::Param::StepEndLevel + step,
                        ControlsT::Param::LevelRange, "End level");
      config_toggle<2>(this, ControlsT::Param::StepTracksEndSource + step,
                       "While generating", {"Ignore changes", "Track changes"},
                       1);

      config_level_knob(this, ControlsT::Param::StepEndLevel + step,
                        ControlsT::Param::LevelRange, "Curve end level");

      config_curve_shape_switch(this, ControlsT::Param::StepShape + step,
                                "Shape");
      config_curvature_knob(this, ControlsT::Param::StepCurvature + step,
                            "Curvature");
      config_duration_knob(this, ControlsT::Param::StepDuration + step,
                           ControlsT::Param::DurationRange, "Duration");
      config_button(this, ControlsT::Param::EnableStep + step, "Enabled",
                    {"From input", "Yes"}, 1);

      controls_.show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    curve_sequencer_.execute(args.sampleTime);
  }

private:
  PhaseTimer timer_{};
  ControlsT controls_{inputs, outputs, params, lights};
  SourceController<ControlsT> start_source_{controls_};
  SourceController<ControlsT> end_source_{controls_};
  Interrupter<ControlsT> interrupter_{controls_};
  Generator<ControlsT, decltype(start_source_)> generator_{
      controls_, start_source_, end_source_};
  Sustainer<ControlsT> sustainer_{controls_};
  StepController<decltype(interrupter_), decltype(generator_),
                 decltype(sustainer_)>
      step_controller_{interrupter_, generator_, sustainer_};
  StepSelector<ControlsT> selector_{controls_, N};

  SequenceController<ControlsT, decltype(selector_), decltype(step_controller_)>
      curve_sequencer_{controls_, selector_, step_controller_};
};
} // namespace curve_sequencer

} // namespace dhe
