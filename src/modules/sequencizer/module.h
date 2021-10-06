#pragma once
#include "./advancement.h"
#include "./anchor.h"
#include "./control-ids.h"
#include "./controls.h"
#include "./generator.h"
#include "./sequence-controller.h"
#include "./step-controller.h"
#include "./step-selector.h"
#include "components/cxmath.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "widgets.h"

#include <engine/Module.hpp>
#include <jansson.h>

namespace dhe {
namespace sequencizer {

template <int N> class Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;

public:
  Module() {
    config(Param::Count, Input::Count, Output::Count, Light::Count);

    config_button(this, Param::Run, "Run", 1);
    configInput(Input::Run, "Run");
    config_button(this, Param::Gate, "Gate");
    configInput(Input::Gate, "Gate");
    config_button(this, Param::Loop, "Loop");
    configInput(Input::Loop, "Loop");
    config_button(this, Param::Reset, "Reset");
    configInput(Input::Reset, "Reset");

    auto selection_start_knob =
        configParam(Param::SelectionStart, 0.F, static_cast<float>(N - 1), 0.F,
                    "Start step", "", 0.F, 1.F, 1.F);
    selection_start_knob->snapEnabled = true;
    auto selection_length_knob = configParam(Param::SelectionLength, 1.F, N, N,
                                             "Sequence length", " steps");
    selection_length_knob->snapEnabled = true;

    config_attenuator(this, Param::LevelMultiplier, "Level multiplier");
    config_level_range_switch(this, Param::LevelRange);
    configInput(Input::LevelAttenuationCV, "Level multiplier CV");

    config_gain(this, Param::DurationMultiplier, "Duration multiplier");
    config_duration_range_switch(this, Param::DurationRange);
    configInput(Input::DurationMultiplierCV, "Duration multipler CV");

    configInput(Input::InA, "A");
    configInput(Input::InB, "B");
    configInput(Input::InC, "C");

    configOutput(Output::StepNumber, "Step number");
    configOutput(Output::IsCurving, "Is curving");
    configOutput(Output::IsSustaining, "Is sustaining");
    configOutput(Output::StepEventPulse, "End of step");
    configOutput(Output::SequenceEventPulse, "Start of sequence");
    configOutput(Output::Out, "Sequencer");

    for (auto step = 0; step < N; step++) {
      auto const step_name = "Step " + std::to_string(step + 1) + " ";
      Stepper<TriggerModes>::config(this, Param::StepTriggerMode + step,
                                    step_name + "trigger mode", 0);
      Stepper<InterruptModes>::config(this, Param::StepInterruptMode + step,
                                      step_name + "interrupt mode", 0);
      Stepper<SustainModes>::config(this, Param::StepSustainMode + step,
                                    step_name + "sustain mode", 0);

      Stepper<AnchorModes>::config(this, Param::StepStartAnchorMode + step,
                                   step_name + "start anchor mode", 0);
      config_level_knob(this, Param::StepStartAnchorLevel + step,
                        Param::LevelRange, "Start level");
      Stepper<AnchorSources>::config(this, Param::StepStartAnchorSource + step,
                                     step_name + "start anchor source", 4);

      Stepper<AnchorModes>::config(this, Param::StepEndAnchorMode + step,
                                   step_name + "end anchor mode", 1);
      config_level_knob(this, Param::StepEndAnchorLevel + step,
                        Param::LevelRange, "End level");
      Stepper<AnchorSources>::config(this, Param::StepEndAnchorSource + step,
                                     step_name + "end anchor source", 0);

      config_curvature_knob(this, Param::StepCurvature + step, "Curvature");
      config_duration_knob(this, Param::StepDuration + step,
                           Param::DurationRange, "Duration");
      Stepper<Shapes>::config(this, Param::StepShape + step,
                              step_name + "shape", 0);
      config_button(this, Param::StepEnabled + step, "Enabled", 1);

      controls_.show_inactive(step);
    }
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    sequence_controller_.execute(args.sampleTime);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  using ControlsT = Controls<N, rack::engine::Input, rack::engine::Param,
                             rack::engine::Output, rack::engine::Light>;
  using AnchorT = Anchor<ControlsT>;
  using GeneratorT = Generator<ControlsT, AnchorT>;
  using InterrupterT = Interrupter<ControlsT>;
  using StepSelectorT = StepSelector<ControlsT>;
  using SustainerT = Sustainer<ControlsT>;

  ControlsT controls_{inputs, params, outputs, lights};
  AnchorT end_anchor_{controls_, AnchorType::End};
  AnchorT start_anchor_{controls_, AnchorType::Start};
  GeneratorT generator_{controls_, start_anchor_, end_anchor_};
  InterrupterT interrupter_{controls_};
  StepSelectorT step_selector_{controls_, N};
  SustainerT sustainer_{controls_};

  using StepControllerT = StepController<InterrupterT, GeneratorT, SustainerT>;
  using SequenceControllerT =
      SequenceController<ControlsT, StepSelectorT, StepControllerT>;

  StepControllerT step_controller_{interrupter_, generator_, sustainer_};
  SequenceControllerT sequence_controller_{controls_, step_selector_,
                                           step_controller_};
};
} // namespace sequencizer

} // namespace dhe
