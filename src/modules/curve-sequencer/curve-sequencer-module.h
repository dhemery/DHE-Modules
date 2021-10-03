#pragma once

#include "components/phase-timer.h"
#include "config/common-config.h"
#include "config/curvature-config.h"
#include "config/duration-config.h"
#include "config/level-config.h"
#include "curve-sequencer-controls.h"
#include "curve-sequencer.h"
#include "generate-mode.h"
#include "step-controller.h"
#include "step-selector.h"

#include <engine/Module.hpp>

namespace dhe {

namespace curve_sequencer {
auto constexpr default_generate_mode = static_cast<int>(GenerateMode::Curve);
auto constexpr generate_mode_descriptions =
    std::array<char const *, generate_mode_count>{"Curve", "Hold",  "Sustain",
                                                  "Input", "Chase", "Level"};

auto constexpr default_advance_mode =
    static_cast<int>(AdvanceMode::TimerExpires);
auto constexpr advance_mode_descriptions =
    std::array<char const *, advance_mode_count>{
        "Timer expires", "Gate rises",   "Gate falls",
        "Gate changes",  "Gate is high", "Gate is low"};

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

    configParam(Controls::SelectionStartKnob, 0.F, N - 1, 0.F, "Start step", "",
                0.F, 1.F, 1.F);
    configParam(Controls::SelectionLengthKnob, 1.F, N, N, "Sequence length",
                " steps");

    config_level_range_switch(this, Controls::LevelRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);

    for (auto step = 0; step < N; step++) {
      auto const prefix = std::string{"Step "} + std::to_string(step + 1) + " ";
      configLight(Controls::ProgressLights + step + step, prefix + "progress");
      config_toggle<generate_mode_count>(
          this, Controls::ModeSwitches + step, prefix + "generate mode",
          generate_mode_descriptions, default_generate_mode);
      config_toggle<advance_mode_count>(
          this, Controls::ConditionSwitches + step, prefix + "advance mode",
          advance_mode_descriptions, default_advance_mode);
      config_level_knob(this, Controls::LevelKnobs + step,
                        Controls::LevelRangeSwitch, prefix + "level");
      config_curve_shape_switch(this, Controls::ShapeSwitches + step,
                                prefix + "shape");
      config_curvature_knob(this, Controls::CurveKnobs + step,
                            prefix + "curvature");
      config_duration_knob(this, Controls::DurationKnobs + step,
                           Controls::DurationRangeSwitch, prefix + "duration");
      config_button(this, Controls::EnabledButtons + step, prefix + "enabled",
                    1);
      configInput(Controls::EnabledInputs + step, prefix + "enabled");

      controls_.show_inactive(step);
    }

    configInput(Controls::CurveSequencerInput, "AUX");
    configOutput(Controls::CurveSequencerOutput, "Main");
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
