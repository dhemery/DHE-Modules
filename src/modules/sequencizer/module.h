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
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/duration-params.h"
#include "params/presets.h"
#include "signals/curvature.h"
#include "signals/duration-signals.h"
#include "signals/step-selection.h"
#include "signals/voltages.h"

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

    Knob::config<SelectionStart<N>>(this, ParamId::SelectionStart, "Start step",
                                    0);
    Knob::config<SelectionLength<N>>(this, ParamId::SelectionLength,
                                     "Sequence length", N);

    configInput(InputId::InA, "A");
    configInput(InputId::InB, "B");
    configInput(InputId::InC, "C");

    configOutput(OutputId::StepNumber, "Step number");
    configOutput(OutputId::IsCurving, "Is curving");
    configOutput(OutputId::IsSustaining, "Is sustaining");
    configOutput(OutputId::StepEventPulse, "End of step");
    configOutput(OutputId::SequenceEventPulse, "Start of sequence");
    configOutput(OutputId::Out, "Sequencer");

    auto level_knobs = std::vector<MappedKnobQuantity<Voltage> *>{};
    auto duration_knobs = std::vector<DurationKnob *>{};

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
          Knob::config<Voltage>(this, ParamId::StepStartAnchorLevel + step,
                                step_name + "start level");
      level_knobs.push_back(start_level_knob);
      Switch::config<AnchorSources>(this, ParamId::StepStartAnchorSource + step,
                                    step_name + "start anchor source",
                                    AnchorSource::Out);

      Switch::config<AnchorModes>(this, ParamId::StepEndAnchorMode + step,
                                  step_name + "end anchor mode",
                                  AnchorMode::Track);
      auto *end_level_knob = Knob::config<Voltage>(
          this, ParamId::StepEndAnchorLevel + step, step_name + "end level");
      level_knobs.push_back(end_level_knob);
      Switch::config<AnchorSources>(this, ParamId::StepEndAnchorSource + step,
                                    step_name + "end anchor source",
                                    AnchorSource::Level);

      Knob::config<Curvature>(this, ParamId::StepCurvature + step,
                              step_name + "curvature");
      auto *duration_knob = DurationKnob::config(
          this, ParamId::StepDuration + step, step_name + "duration");
      duration_knobs.push_back(duration_knob);

      Switch::config<Shapes>(this, ParamId::StepShape + step,
                             step_name + "shape", Shape::Id::J);
      Button::config(this, ParamId::StepEnabled + step, step_name + "enabled",
                     1);

      signals_.show_inactive(step);
    }

    Knob::config<Attenuator>(this, ParamId::LevelMultiplier,
                             "Level multiplier");
    auto select_level_range = [level_knobs](VoltageRangeId id) {
      for (auto *knob : level_knobs) {
        knob->mapper().select_range(id);
      }
    };
    Switch::config<VoltageRanges>(this, ParamId::LevelRange, "Level range",
                                  VoltageRangeId::Unipolar)
        ->on_change(select_level_range);
    configInput(InputId::LevelAttenuationCV, "Level multiplier CV");

    Knob::config<Gain>(this, ParamId::DurationMultiplier,
                       "Duration multiplier");
    auto select_duration_range = [duration_knobs](DurationRangeId range_id) {
      for (auto *knob : duration_knobs) {
        knob->select_range(range_id);
      }
    };
    Switch::config<DurationRanges>(this, ParamId::DurationRange,
                                   "Duration range", DurationRangeId::Medium)
        ->on_change(select_duration_range);
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
