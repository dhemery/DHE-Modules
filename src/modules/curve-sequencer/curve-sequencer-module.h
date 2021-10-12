#pragma once

#include "./controls.h"
#include "./curve-sequencer-controls.h"
#include "./curve-sequencer.h"
#include "./generate-mode.h"
#include "./step-controller.h"
#include "./step-selector.h"
#include "components/phase-timer.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "controls/steppers.h"

#include <rack.hpp>

namespace dhe {

namespace curve_sequencer {
template <int N> class CurveSequencerModule : public rack::engine::Module {
  using Controls = CurveSequencerControls<
      std::vector<rack::engine::Input>, std::vector<rack::engine::Output>,
      std::vector<rack::engine::Param>, std::vector<rack::engine::Light>, N>;

public:
  CurveSequencerModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount, Controls::LightCount);

    config_button(this, Controls::RunButton, "Run", 1);
    configInput(Controls::RunInput, "Run");
    config_button(this, Controls::GateButton, "Gate");
    configInput(Controls::GateInput, "Gate");
    config_button(this, Controls::LoopButton, "Loop");
    configInput(Controls::LoopInput, "Loop");
    config_button(this, Controls::ResetButton, "Reset");
    configInput(Controls::ResetInput, "Reset");

    auto const selection_start_knob =
        configParam(Controls::SelectionStartKnob, 0.F, N - 1, 0.F, "Start step",
                    "", 0.F, 1.F, 1.F);
    selection_start_knob->snapEnabled = true;
    auto const selection_length_knob = configParam(
        Controls::SelectionLengthKnob, 1.F, N, N, "Sequence length", " steps");
    selection_length_knob->snapEnabled = true;

    config_level_range_switch(this, Controls::LevelRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);

    for (auto step = 0; step < N; step++) {
      auto const step_name =
          std::string{"Step "} + std::to_string(step + 1) + " ";
      configLight(Controls::ProgressLights + step + step,
                  step_name + "progress");
      Stepper<GenerateModes>::config(this, Controls::ModeSwitches + step,
                                     step_name + "generate mode",
                                     GenerateMode::Curve);
      Stepper<AdvanceModes>::config(this, Controls::ConditionSwitches + step,
                                    step_name + "advance mode",
                                    AdvanceMode::TimerExpires);
      config_level_knob(this, Controls::LevelKnobs + step,
                        Controls::LevelRangeSwitch, step_name + "level");
      config_curve_shape_switch(this, Controls::ShapeSwitches + step,
                                step_name + "shape");
      config_curvature_knob(this, Controls::CurveKnobs + step,
                            step_name + "curvature");
      config_duration_knob(this, Controls::DurationKnobs + step,
                           Controls::DurationRangeSwitch,
                           step_name + "duration");
      config_button(this, Controls::EnabledButtons + step,
                    step_name + "enabled", 1);
      configInput(Controls::EnabledInputs + step, step_name + "enabled");

      controls_.show_inactive(step);
    }

    configInput(Controls::CurveSequencerInput, "AUX");
    configOutput(Controls::CurveSequencerOutput, "Sequencer");
  }

  ~CurveSequencerModule() override = default;

  void process(ProcessArgs const &args) override {
    curve_sequencer_.execute(args.sampleTime);
  }

private:
  PhaseTimer timer_{};
  Controls controls_{inputs, outputs, params, lights};
  StepController<Controls> step_controller_{controls_, timer_};
  StepSelector<Controls> selector_{controls_, N};

  CurveSequencer<Controls, StepSelector<Controls>, StepController<Controls>>
      curve_sequencer_{controls_, selector_, step_controller_};
};
} // namespace curve_sequencer

} // namespace dhe
