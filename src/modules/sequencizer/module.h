#pragma once

#include "anchor.h"
#include "control-ids.h"
#include "generator.h"
#include "sequence-controller.h"
#include "signals.h"
#include "step-controller.h"
#include "step-selector.h"
#include "trigger-mode.h"

#include "components/cxmath.h"
#include "controls/buttons.h"
#include "controls/duration-controls.h"
#include "controls/knobs.h"
#include "controls/shape-controls.h"
#include "controls/step-selection-controls.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/duration-signals.h"
#include "signals/shape-signals.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

#include <string>

namespace dhe {
namespace sequencizer {

template <int N> class Module : public rack::engine::Module {
public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count, LightId::Count);

    Button::config(this, ParamId::Run, "Run", 1);
    configInput(InputId::Run, "Run");
    Button::config(this, ParamId::Gate, "Gate");
    configInput(InputId::Gate, "Gate");
    Button::config(this, ParamId::Loop, "Loop");
    configInput(InputId::Loop, "Loop");
    Button::config(this, ParamId::Reset, "Reset");
    configInput(InputId::Reset, "Reset");

    SelectionStartKnob::config<N>(this, ParamId::SelectionStart, "Start step");
    SelectionLengthKnob::config<N>(this, ParamId::SelectionLength,
                                   "Sequence length");

    configInput(InputId::InA, "A");
    configInput(InputId::InB, "B");
    configInput(InputId::InC, "C");

    configOutput(OutputId::StepNumber, "Step number");
    configOutput(OutputId::IsCurving, "Is curving");
    configOutput(OutputId::IsSustaining, "Is sustaining");
    configOutput(OutputId::StepEventPulse, "End of step");
    configOutput(OutputId::SequenceEventPulse, "Start of sequence");
    configOutput(OutputId::Out, "Sequencer");

    auto *duration_range_switch =
        DurationRangeSwitch::config(this, ParamId::DurationRange,
                                    "Duration range", DurationRangeId::Medium);
    auto *level_range_switch = VoltageRangeSwitch::config(
        this, ParamId::LevelRange, "Level range", VoltageRangeId::Unipolar);

    for (auto step = 0; step < N; step++) {
      auto const step_name = "Step " + std::to_string(step + 1) + " ";
      Switch::config<TriggerModes>(this, ParamId::StepTriggerMode + step,
                                   step_name + "trigger mode",
                                   TriggerMode::GateRises);
      Switch::config<InterruptModes>(this, ParamId::StepInterruptMode + step,
                                     step_name + "interrupt mode",
                                     InterruptMode::No);
      Switch::config<SustainModes>(this, ParamId::StepSustainMode + step,
                                   step_name + "sustain mode", SustainMode::No);

      Switch::config<AnchorModes>(this, ParamId::StepStartAnchorMode + step,
                                  step_name + "start anchor mode",
                                  AnchorMode::Sample);
      auto *start_level_knob =
          VoltageKnob::config(this, ParamId::StepStartAnchorLevel + step,
                              step_name + "start level");
      level_range_switch->add_knob(start_level_knob);
      Switch::config<AnchorSources>(this, ParamId::StepStartAnchorSource + step,
                                    step_name + "start anchor source",
                                    AnchorSource::Out);

      Switch::config<AnchorModes>(this, ParamId::StepEndAnchorMode + step,
                                  step_name + "end anchor mode",
                                  AnchorMode::Track);
      auto *end_level_knob = VoltageKnob::config(
          this, ParamId::StepEndAnchorLevel + step, step_name + "end level");
      level_range_switch->add_knob(end_level_knob);
      Switch::config<AnchorSources>(this, ParamId::StepEndAnchorSource + step,
                                    step_name + "end anchor source",
                                    AnchorSource::Level);

      CurvatureKnob::config(this, ParamId::StepCurvature + step,
                            step_name + "curvature");
      auto *duration_knob = DurationKnob::config(
          this, ParamId::StepDuration + step, step_name + "duration");
      duration_range_switch->add_knob(duration_knob);

      ShapeSwitch::config(this, ParamId::StepShape + step, step_name + "shape",
                          Shape::Id::J);
      Button::config(this, ParamId::StepEnabled + step, step_name + "enabled",
                     1);

      signals_.show_inactive(step);
    }

    PercentageKnob::config(this, ParamId::LevelMultiplier, "Level multiplier");
    configInput(InputId::LevelAttenuationCV, "Level multiplier CV");

    GainKnob::config(this, ParamId::DurationMultiplier, "Duration multiplier");
    configInput(InputId::DurationMultiplierCV, "Duration multipler CV");
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

  using ParamId = ParamIds<N>;
  using LightId = LightIds<N>;
};
} // namespace sequencizer

} // namespace dhe
