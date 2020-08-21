#pragma once

#include "CurveSequencer.h"
#include "CurveSequencerControls.h"
#include "GenerateMode.h"
#include "StepController.h"
#include "StepSelector.h"
#include "components/PhaseTimer.h"
#include "config/CommonConfig.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

#include <engine/Module.hpp>

namespace dhe {

namespace curve_sequencer {
static auto constexpr defaultGenerateMode =
    static_cast<int>(GenerateMode::Curve);
static auto const generateModeDescriptions =
    std::array<std::string, generateModeCount>{"Curve", "Hold",  "Sustain",
                                               "Input", "Chase", "Level"};

static auto constexpr defaultAdvanceMode =
    static_cast<int>(AdvanceMode::TimerExpires);
static auto const advanceModeDescriptions =
    std::array<std::string, advanceModeCount>{"Timer expires", "Gate rises",
                                              "Gate falls",    "Gate changes",
                                              "Gate is high",  "Gate is low"};

template <int N> class CurveSequencerModule : public rack::engine::Module {
  using Controls =
      CurveSequencerControls<rack::engine::Input, rack::engine::Output,
                             rack::engine::Param, rack::engine::Light, N>;

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

    configLevelRangeSwitch(this, Controls::LevelRangeSwitch);
    configDurationRangeSwitch(this, Controls::DurationRangeSwitch);

    for (auto step = 0; step < N; step++) {
      config_toggle<generateModeCount>(
          this, Controls::ModeSwitches + step, "Generate Mode",
          generateModeDescriptions, defaultGenerateMode);
      config_toggle<advanceModeCount>(this, Controls::ConditionSwitches + step,
                                      "Advance Mode", advanceModeDescriptions,
                                      defaultAdvanceMode);
      configLevelKnob(this, Controls::LevelKnobs + step,
                      Controls::LevelRangeSwitch, "Level");
      config_curve_shape_switch(this, Controls::ShapeSwitches + step, "Shape");
      config_curvature_knob(this, Controls::CurveKnobs + step, "Curvature");
      configDurationKnob(this, Controls::DurationKnobs + step,
                         Controls::DurationRangeSwitch, "Duration");
      config_button(this, Controls::EnabledButtons + step, "Enabled",
                    {"from input", "Yes"}, 1);

      controls.showInactive(step);
    }
  }

  ~CurveSequencerModule() override = default;

  void process(ProcessArgs const &args) override {
    curveSequencer.execute(args.sampleTime);
  }

private:
  PhaseTimer timer{};
  Controls controls{inputs, outputs, params, lights};
  StepController<Controls> stepController{controls, timer};
  StepSelector<Controls> selector{controls, N};

  CurveSequencer<Controls, decltype(selector), decltype(stepController)>
      curveSequencer{controls, selector, stepController};
};
} // namespace curve_sequencer

} // namespace dhe
