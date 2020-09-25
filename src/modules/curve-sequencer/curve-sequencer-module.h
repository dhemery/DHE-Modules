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

    config_button(this, Controls::RunButton, "Run", {"from input", "Yes"}, 1);
    config_button(this, Controls::GateButton, "Gate", {"from input", "High"},
                  0);
    config_button(this, Controls::LoopButton, "Loop", {"from input", "Yes"}, 0);
    config_button(this, Controls::ResetButton, "Reset", {"from input", "High"},
                  0);

    configParam(Controls::SelectionStartKnob, 0.F, N - 1, 0.F, "Start step", "",
                0.F, 1.F, 1.F);
    configParam(Controls::SelectionLengthKnob, 1.F, N, N, "Sequence length",
                " steps");

    config_level_range_switch(this, Controls::LevelRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);

    for (auto step = 0; step < N; step++) {
      config_toggle<generate_mode_count>(
          this, Controls::ModeSwitches + step, "Generate Mode",
          generate_mode_descriptions, default_generate_mode);
      config_toggle<advance_mode_count>(
          this, Controls::ConditionSwitches + step, "Advance Mode",
          advance_mode_descriptions, default_advance_mode);
      config_level_knob(this, Controls::LevelKnobs + step,
                        Controls::LevelRangeSwitch, "Level");
      config_curve_shape_switch(this, Controls::ShapeSwitches + step, "Shape");
      config_curvature_knob(this, Controls::CurveKnobs + step, "Curvature");
      config_duration_knob(this, Controls::DurationKnobs + step,
                           Controls::DurationRangeSwitch, "Duration");
      config_button(this, Controls::EnabledButtons + step, "Enabled",
                    {"from input", "Yes"}, 1);

      controls_.show_inactive(step);
    }
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
