#pragma once

#include "CurveSequencer.h"
#include "GeneratingMode.h"
#include "SustainingMode.h"
#include "config/CommonConfig.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

#include <engine/Module.hpp>

namespace dhe {

namespace curve_sequencer {
  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Output;
  using rack::engine::Param;

  template <int N> class CurveSequencerModule : public rack::engine::Module {
    using Controls = CurveSequencerControls<N>;

  public:
    CurveSequencerModule() {
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount, Controls::LightCount);

      configButton(this, Controls::RunButton, "Run", {"RUN input", "Yes"}, 1);
      configButton(this, Controls::GateButton, "Gate", {"GATE input", "High"}, 0);
      configButton(this, Controls::ResetButton, "Reset", {"RESET input", "High"}, 0);

      configParam(Controls::StartKnob, 1.F, N, 1.F, "Start step");
      configParam(Controls::StepsKnob, 1.F, N, N, "Sequence length", " steps");

      configLevelRangeSwitch(this, Controls::LevelRangeSwitch);
      configDurationRangeSwitch(this, Controls::DurationRangeSwitch);

      for (int stepIndex = 0; stepIndex < N; stepIndex++) {
        configToggle<generatingInterruptModeCount>(this, Controls::GenerateModeSwitches + stepIndex, "Generate mode",
                                                   generatingInterruptModeDescriptions, defaultGeneratingInterruptMode);
        configToggle<sustainingInterruptModeCount>(this, Controls::SustainModeSwitches + stepIndex, "Sustain mode",
                                                   sustainingInterruptModeDescriptions, defaultSustainingInterruptMode);
        configLevelKnob(this, Controls::LevelKnobs + stepIndex, Controls::LevelRangeSwitch, "Level");
        configCurveShapeSwitch(this, Controls::ShapeSwitches + stepIndex, "Shape");
        configCurvatureKnob(this, Controls::CurveKnobs + stepIndex, "Curvature");
        configDurationKnob(this, Controls::DurationKnobs + stepIndex, Controls::DurationRangeSwitch, "Duration");
        configButton(this, Controls::EnabledButtons + stepIndex, "Enabled", {"ENABLED input", "Yes"}, 1);

        lights[Controls::GeneratingLights + stepIndex].setBrightness(0.F);
        lights[Controls::SustainingLights + stepIndex].setBrightness(0.F);
      }
    }

    ~CurveSequencerModule() override = default;

    void process(const ProcessArgs &args) override { curveSequencer.execute(args.sampleTime); }

  private:
    CurveSequencer<N, Input, Output, Param, Light> curveSequencer{inputs, outputs, params, lights};
  };
} // namespace curve_sequencer

} // namespace dhe
