#pragma once

#include "advance-mode.h"
#include "control-ids.h"
#include "engine.h"
#include "generate-mode.h"
#include "signals.h"
#include "step-controller.h"
#include "step-selector.h"

#include "components/phase-timer.h"
#include "components/sigmoid.h"
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
#include <vector>

namespace dhe {

namespace curve_sequencer {
template <int N> struct Module : rack::engine::Module {
  using ParamId = ParamIds<N>;
  using InputId = InputIds<N>;
  using LightId = LightIds<N>;

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

    auto *duration_range_switch =
        DurationRangeSwitch::config(this, ParamId::DurationRange,
                                    "Duration range", DurationRangeId::Medium);

    auto *level_range_switch = VoltageRangeSwitch::config(
        this, ParamId::LevelRange, "Level range", VoltageRangeId::Unipolar);

    for (auto step = 0; step < N; step++) {
      auto const step_name =
          std::string{"Step "} + std::to_string(step + 1) + " ";
      configLight(LightId::StepProgress + step + step, step_name + "progress");
      Switch::config<GenerateModes>(this, ParamId::StepGenerateMode + step,
                                    step_name + "generate mode",
                                    GenerateMode::Curve);
      Switch::config<AdvanceModes>(this, ParamId::StepAdvanceMode + step,
                                   step_name + "advance mode",
                                   AdvanceMode::TimerExpires);
      auto *level_knob = VoltageKnob::config(this, ParamId::StepLevel + step,
                                             step_name + "level");
      level_range_switch->add_knob(level_knob);

      ShapeSwitch::config(this, ParamId::StepShape + step, step_name + "shape",
                          Shape::Id::J);
      CurvatureKnob::config(this, ParamId::StepCurvature + step,
                            step_name + "curvature");
      auto *duration_knob = DurationKnob::config(
          this, ParamId::StepDuration + step, step_name + "duration");
      duration_range_switch->add_knob(duration_knob);

      Button::config(this, ParamId::StepEnabled + step, step_name + "enabled",
                     1);
      configInput(InputId::StepEnabled + step, step_name + "enabled");

      signals_.show_inactive(step);
    }

    configInput(InputId::Main, "AUX");
    configOutput(OutputId::Main, "Sequencer");
  }

  ~Module() override = default;

  void process(ProcessArgs const &args) override {
    engine_.execute(args.sampleTime);
  }

private:
  using SignalsType = Signals<rack::engine::Param, rack::engine::Input,
                              rack::engine::Output, rack::engine::Light, N>;
  PhaseTimer timer_{};
  SignalsType signals_{params, inputs, outputs, lights};
  StepController<SignalsType> step_controller_{signals_, timer_};
  StepSelector<SignalsType> selector_{signals_, N};

  Engine<SignalsType, StepSelector<SignalsType>, StepController<SignalsType>>
      engine_{signals_, selector_, step_controller_};
};
} // namespace curve_sequencer

} // namespace dhe
