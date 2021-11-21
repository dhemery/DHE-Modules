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
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/curvature.h"
#include "signals/durations.h"
#include "signals/shapes.h"
#include "signals/step-selection.h"
#include "signals/voltages.h"

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

    Knob::config<SelectionStart<N>>(this, ParamId::SelectionStart, "Start step",
                                    0);
    Knob::config<SelectionLength<N>>(this, ParamId::SelectionLength,
                                     "Sequence length", N);

    auto level_knobs = std::vector<MappedKnobQuantity<VoltageRanges> *>{};
    auto duration_knobs = std::vector<MappedKnobQuantity<DurationRanges> *>{};

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
      auto *level_knob = Knob::config<VoltageRanges>(
          this, ParamId::StepLevel + step, step_name + "level");
      level_knobs.push_back(level_knob);

      Switch::config<Shapes>(this, ParamId::StepShape + step,
                             step_name + "shape", Shape::Id::J);
      Knob::config<Curvature>(this, ParamId::StepCurvature + step,
                              step_name + "curvature");
      auto *duration_knob = Knob::config<DurationRanges>(
          this, ParamId::StepDuration + step, step_name + "duration");
      duration_knobs.push_back(duration_knob);

      Button::config(this, ParamId::StepEnabled + step, step_name + "enabled",
                     1);
      configInput(InputId::StepEnabled + step, step_name + "enabled");

      signals_.show_inactive(step);
    }

    auto select_level_range = [level_knobs](VoltageRangeId id) {
      for (auto *knob : level_knobs) {
        knob->mapper().select_range(id);
      }
    };
    Switch::config<VoltageRanges>(this, ParamId::LevelRange, "Level range",
                                  VoltageRangeId::Unipolar)
        ->on_change(select_level_range);

    auto select_duration_range = [duration_knobs](DurationRangeId id) {
      for (auto *knob : duration_knobs) {
        knob->mapper().select_range(id);
      }
    };
    Switch::config<DurationRanges>(this, ParamId::DurationRange,
                                   "Duration range", DurationRangeId::Medium)
        ->on_change(select_duration_range);

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
