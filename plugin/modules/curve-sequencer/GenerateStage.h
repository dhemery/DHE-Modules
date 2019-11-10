#pragma once

#include "CurveSequencerControls.h"
#include "SequenceMode.h"
#include "SequencerState.h"
#include "StageMode.h"
#include "components/Latch.h"
#include "components/OneShotPhaseAccumulator.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"

#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Output;
  using rack::engine::Param;

  template <int N, typename Phase = OneShotPhaseAccumulator> class GenerateStage {
    using Controls = CurveSequencerControls<N>;

  public:
    GenerateStage(std::vector<Input> &inputs, std::vector<Output> &outputs, std::vector<Param> &params,
                  std::vector<Light> &lights, Phase &phase) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights}, phase{phase} {}

    void enter(int entryStep) {
      step = entryStep;
      phase.reset();
      startVoltage = outputs[Controls::CurveSequencerOutput].getVoltage();
      setLight(true);
    }

    auto execute(dhe::Latch const &gateLatch, float sampleTime) -> SequencerState {
      if (isActive(runMode(), gateLatch)) {
        generate(sampleTime);
        if (phase.state() == OneShotPhaseAccumulator::State::Incomplete) {
          return {SequenceMode::Generating, step};
        }
      }
      setLight(false);
      return {SequenceMode::Sustaining, step};
    };

    void exit() { setLight(false); }

  private:
    auto duration() const -> float {
      return selectableDuration(params[Controls::DurationKnobs + step], params[Controls::DurationRangeSwitch]);
    }

    auto level() const -> float {
      return selectableLevel(params[Controls::LevelKnobs + step], params[Controls::LevelRangeSwitch]);
    }

    void generate(float sampleTime) {
      phase.advance(sampleTime / duration());
      outputs[Controls::CurveSequencerOutput].setVoltage(scale(taper(phase.phase()), startVoltage, level()));
    }

    auto runMode() const -> StageMode {
      return static_cast<StageMode>(params[Controls::GenerateModeSwitches + step].getValue());
    }

    void setLight(const bool state) const {
      lights[Controls::GeneratingLights + step].setBrightness(state ? 10.F : 0.F);
    }

    auto taper(float input) const -> float {
      auto const curvature = dhe::curvature(params[Controls::CurveKnobs + step]);
      auto const taper = selectedTaper(params[Controls::ShapeSwitches + step]);
      return taper->apply(input, curvature);
    }

    std::vector<Input> &inputs;
    std::vector<Output> &outputs;
    std::vector<Param> &params;
    std::vector<Light> &lights;
    Phase &phase;
    int step;
    float startVoltage;
  }; // namespace curve_sequencer
};   // namespace curve_sequencer
} // namespace dhe
