#pragma once

#include "CurveSequencer.h"
#include "CurveSequencerControls.h"
#include "StepController.h"
#include "StepMode.h"
#include "StepSelector.h"
#include "components/PhaseTimer.h"
#include "config/CommonConfig.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

#include <engine/Module.hpp>

namespace dhe {

namespace curve_sequencer {
  static auto constexpr defaultMode = static_cast<int>(StepMode::Curve);
  static auto const modeDescriptions = std::array<std::string, modeCount>{"Curve", "Hold", "Sustain"};

  static auto constexpr defaultAdvanceCondition = static_cast<int>(AdvanceCondition::TimerExpires);
  static auto const advanceConditionDescriptions = std::array<std::string, advanceConditionCount>{
      "Timer expires", "Gate rises", "Gate falls", "Gate changes", "Gate is high", "Gate is low"};

  template <int N> class CurveSequencerModule : public rack::engine::Module {
    using Controls = CurveSequencerControls<N>;

  public:
    CurveSequencerModule() {
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount, Controls::LightCount);

      configButton(this, Controls::RunButton, "Run", {"from input", "Yes"}, 1);
      configButton(this, Controls::GateButton, "Gate", {"from input", "High"}, 0);
      configButton(this, Controls::LoopButton, "Loop", {"from input", "Yes"}, 0);
      configButton(this, Controls::ResetButton, "Reset", {"from input", "High"}, 0);

      configParam(Controls::SelectionStartKnob, 0.F, N - 1, 0.F, "Start step", "", 0.F, 1.F, 1.F);
      configParam(Controls::SelectionLengthKnob, 1.F, N, N, "Sequence length", " steps");

      configLevelRangeSwitch(this, Controls::LevelRangeSwitch);
      configDurationRangeSwitch(this, Controls::DurationRangeSwitch);

      for (int step = 0; step < N; step++) {
        configToggle<modeCount>(this, Controls::ModeSwitches + step, "Mode", modeDescriptions, defaultMode);
        configToggle<advanceConditionCount>(this, Controls::ConditionSwitches + step, "Advance when",
                                            advanceConditionDescriptions, defaultAdvanceCondition);
        configLevelKnob(this, Controls::LevelKnobs + step, Controls::LevelRangeSwitch, "Level");
        configCurveShapeSwitch(this, Controls::ShapeSwitches + step, "Shape");
        configCurvatureKnob(this, Controls::CurveKnobs + step, "Curvature");
        configDurationKnob(this, Controls::DurationKnobs + step, Controls::DurationRangeSwitch, "Duration");
        configButton(this, Controls::EnabledButtons + step, "Enabled", {"from input", "Yes"}, 1);

        controls.showInactive(step);
      }
    }

    ~CurveSequencerModule() override = default;

    void process(const ProcessArgs &args) override { curveSequencer.execute(args.sampleTime); }

  private:
    PhaseTimer timer{};
    Controls controls{inputs, outputs, params, lights};
    StepController<Controls> stepController{controls, timer};
    StepSelector<Controls> selector{controls, N};

    CurveSequencer<Controls, decltype(selector), decltype(stepController)> curveSequencer{controls, selector,
                                                                                          stepController};
  };
} // namespace curve_sequencer

} // namespace dhe
