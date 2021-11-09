#pragma once
#include "advancement.h"
#include "anchor.h"
#include "control-ids.h"
#include "generator.h"
#include "params.h"
#include "sequence-controller.h"
#include "signals.h"
#include "step-controller.h"
#include "step-selector.h"

#include "components/cxmath.h"
#include "controls/buttons.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/curvature-knob-quantity.h"
#include "params/duration-knob-quantity.h"
#include "params/presets.h"
#include "signals/durations.h"
#include "signals/shapes.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace sequencizer {

template <int N> class Module : public rack::engine::Module {
public:
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

    configInput(Input::InA, "A");
    configInput(Input::InB, "B");
    configInput(Input::InC, "C");

    configOutput(Output::StepNumber, "Step number");
    configOutput(Output::IsCurving, "Is curving");
    configOutput(Output::IsSustaining, "Is sustaining");
    configOutput(Output::StepEventPulse, "End of step");
    configOutput(Output::SequenceEventPulse, "Start of sequence");
    configOutput(Output::Out, "Sequencer");

    auto level_knobs = std::vector<KnobQuantity<float> *>{};
    auto duration_knobs = std::vector<DurationKnobQuantity *>{};

    for (auto step = 0; step < N; step++) {
      auto const step_name = "Step " + std::to_string(step + 1) + " ";
      Switch::config<TriggerModes>(this, Param::StepTriggerMode + step,
                                   step_name + "trigger mode",
                                   TriggerMode::GateRises);
      Switch::config<InterruptModes>(this, Param::StepInterruptMode + step,
                                     step_name + "interrupt mode",
                                     InterruptMode::No);
      Switch::config<SustainModes>(this, Param::StepSustainMode + step,
                                   step_name + "sustain mode", SustainMode::No);

      Switch::config<AnchorModes>(this, Param::StepStartAnchorMode + step,
                                  step_name + "start anchor mode",
                                  AnchorMode::Sample);
      auto *start_level_knob = Knob::config<UnipolarVoltage>(
          this, Param::StepStartAnchorLevel + step, step_name + "start level");
      level_knobs.push_back(start_level_knob);
      Switch::config<AnchorSources>(this, Param::StepStartAnchorSource + step,
                                    step_name + "start anchor source",
                                    AnchorSource::Out);

      Switch::config<AnchorModes>(this, Param::StepEndAnchorMode + step,
                                  step_name + "end anchor mode",
                                  AnchorMode::Track);
      auto *end_level_knob = Knob::config<UnipolarVoltage>(
          this, Param::StepEndAnchorLevel + step, step_name + "end level");
      level_knobs.push_back(end_level_knob);
      Switch::config<AnchorSources>(this, Param::StepEndAnchorSource + step,
                                    step_name + "end anchor source",
                                    AnchorSource::Level);

      CurvatureKnob::config(this, Param::StepCurvature + step,
                            step_name + "curvature");
      auto *duration_knob = DurationKnob::config(
          this, Param::StepDuration + step, step_name + "duration");
      duration_knobs.push_back(duration_knob);

      Switch::config<Shapes>(this, Param::StepShape + step, step_name + "shape",
                             0);
      Button::config(this, Param::StepEnabled + step, step_name + "enabled", 1);

      signals_.show_inactive(step);
    }

    Knob::config<Attenuator>(this, Param::LevelMultiplier, "Level multiplier",
                             1.F);
    Picker::config<Voltages>(this, Param::LevelRange, "Level range",
                             Voltages::Unipolar)
        ->on_change([level_knobs](Range r) {
          for (auto *knob : level_knobs) {
            knob->set_display_range(r);
          }
        });
    configInput(Input::LevelAttenuationCV, "Level multiplier CV");

    Knob::config<Gain>(this, Param::DurationMultiplier, "Duration multiplier",
                       0.5F);
    Picker::config<Durations>(this, Param::DurationRange, "Duration range",
                              Durations::Medium)
        ->on_change([duration_knobs](Range r) {
          for (auto *knob : duration_knobs) {
            knob->set_display_range(r);
          }
        });
    configInput(Input::DurationMultiplierCV, "Duration multipler CV");
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
  using RackSignals = Signals<rack::engine::Param, rack::engine::Input,
                              rack::engine::Output, rack::engine::Light, N>;
  using RackAnchor = Anchor<RackSignals>;
  using RackGenerator = Generator<RackSignals, RackAnchor>;
  using RackInterrupter = Interrupter<RackSignals>;
  using RackStepSelector = StepSelector<RackSignals>;
  using RackSustainer = Sustainer<RackSignals>;
  using RackStepController =
      StepController<RackInterrupter, RackGenerator, RackSustainer>;
  using RackSequenceController =
      SequenceController<RackSignals, RackStepSelector, RackStepController>;

  RackSignals signals_{inputs, params, outputs, lights};
  RackAnchor end_anchor_{signals_, AnchorType::End};
  RackAnchor start_anchor_{signals_, AnchorType::Start};
  RackGenerator generator_{signals_, start_anchor_, end_anchor_};
  RackInterrupter interrupter_{signals_};
  RackStepSelector step_selector_{signals_, N};
  RackSustainer sustainer_{signals_};
  RackStepController step_controller_{interrupter_, generator_, sustainer_};
  RackSequenceController sequence_controller_{signals_, step_selector_,
                                              step_controller_};

  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;
};
} // namespace sequencizer

} // namespace dhe
