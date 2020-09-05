#pragma once

#include "anchor.h"
#include "components/phase-timer.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "control-ids.h"
#include "controls.h"
#include "generator.h"
#include "interrupter.h"
#include "sequence-controller.h"
#include "step-controller.h"
#include "step-selector.h"
#include "sustainer.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curve_sequencer {
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

      config_toggle<3>(this, Param::StepStartAnchorSource + step,
                       "Start anchor source",
                       {"Level knob", "IN port", "OUT port"}, 2);
      config_level_knob(this, Param::StepStartLevel + step, Param::LevelRange,
                        "Start level");
      config_toggle<2>(this, Param::StepStartAnchorMode + step,
                       "Start anchor mode", {"Snapshot", "Track changes"});

      config_toggle<3>(this, Param::StepEndAnchorSource + step,
                       "End anchor source",
                       {"Level knob", "IN port", "OUT port"});
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

      controls_.show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    sequence_controller_.execute(args.sampleTime);
  }

private:
  using ControlsT = Controls<rack::engine::Input, rack::engine::Output,
                             rack::engine::Param, rack::engine::Light, N>;
  using StartAnchorT = StartAnchor<ControlsT>;
  using InterrupterT = Interrupter<ControlsT>;
  using GeneratorT = Generator<ControlsT, StartAnchorT>;
  using SustainerT = Sustainer<ControlsT>;
  using StepControllerT = StepController<InterrupterT, GeneratorT, SustainerT>;
  using StepSelectorT = StepSelector<ControlsT>;
  using SequenceControllerT =
      SequenceController<ControlsT, StepSelectorT, StepControllerT>;

  ControlsT controls_{inputs, outputs, params, lights};
  StartAnchorT start_anchor_{controls_};
  StartAnchorT end_anchor_{controls_};
  InterrupterT interrupter_{controls_};
  GeneratorT generator_{controls_, start_anchor_, end_anchor_};
  SustainerT sustainer_{controls_};
  StepControllerT step_controller_{interrupter_, generator_, sustainer_};
  StepSelectorT step_selector_{controls_, N};
  SequenceControllerT sequence_controller_{controls_, step_selector_,
                                           step_controller_};
};
} // namespace curve_sequencer

} // namespace dhe
