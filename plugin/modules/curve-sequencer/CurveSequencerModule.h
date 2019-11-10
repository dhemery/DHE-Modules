#pragma once

#include "CurveSequencer.h"
#include "GenerateStage.h"
#include "SustainStage.h"
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

  static auto constexpr generateModeCount = static_cast<int>(StageMode::Generate) + 1;
  static auto constexpr defaultGenerateMode = static_cast<int>(StageMode::Generate);
  static auto const generateModeDescriptions = std::array<std::string, generateModeCount>{
      "Generate while gate is high", "Generate while gate is low", "Generate while gate is calm", "Skip generating",
      "Generate until done"};

  static auto constexpr sustainModeCount = static_cast<int>(StageMode::Skip) + 1;
  static auto constexpr defaultSustainMode = static_cast<int>(StageMode::Skip);
  static auto const sustainModeDescriptions = std::array<std::string, sustainModeCount>{
      "Sustain while gate is high", "Sustain while gate is low", "Sustain while gate is calm", "Skip sustain"};

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

      for (int stepIndex = 0; stepIndex < N; stepIndex++) {
        configToggle<generateModeCount>(this, Controls::GenerateModeSwitches + stepIndex, "Generate mode",
                                        generateModeDescriptions, defaultGenerateMode);
        configToggle<sustainModeCount>(this, Controls::SustainModeSwitches + stepIndex, "Sustain mode",
                                       sustainModeDescriptions, defaultSustainMode);
        configLevelKnob(this, Controls::LevelKnobs + stepIndex, Controls::LevelRangeSwitch, "Level");
        configCurveShapeSwitch(this, Controls::ShapeSwitches + stepIndex, "Shape");
        configCurvatureKnob(this, Controls::CurveKnobs + stepIndex, "Curvature");
        configDurationKnob(this, Controls::DurationKnobs + stepIndex, Controls::DurationRangeSwitch, "Duration");
        configButton(this, Controls::EnabledButtons + stepIndex, "Enabled", {"from input", "Yes"}, 1);

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
