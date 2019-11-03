#pragma once

#include "CurveSequencerControls.h"
#include "InterruptModes.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  static auto constexpr generateModeCount = static_cast<int>(InterruptMode::Duration) + 1;
  static auto constexpr generateDefaultMode = static_cast<int>(InterruptMode::Duration);
  static auto constexpr sustainModeCount = static_cast<int>(InterruptMode::Skip) + 1;
  static auto constexpr sustainDefaultMode = static_cast<int>(InterruptMode::Skip);

  static auto const generateModeDescriptions = std::array<std::string, generateModeCount>{
      "Advance if gate rises",       "Advance if gate falls",  "Advance if gate changes",
      "Advance if gate is high",     "Advance if gate is low", "Skip curve",
      "Advance when curve completes"};

  static auto const sustainModeDescriptions = std::array<std::string, sustainModeCount>{
      "Sustain until gate rises",   "Sustain until gate falls",  "Sustain until gate changes",
      "Sustain until gate is high", "Sustain until gate is low", "Skip sustain"};

  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType>
  class GenerateStage {
    using Controls = CurveSequencerControls<N>;

  public:
    GenerateStage(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                  std::vector<LightType> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    auto execute(int step, Latch const &gateLatch, float) -> bool {
      auto const active = isActive(generateMode(step), gateLatch);
      setGenerating(step, active);
      return active;
    }

    void setGenerating(int stepIndex, bool state) {
      lights[Controls::GeneratingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
    }

    auto generateMode(int stepIndex) const -> InterruptMode {
      return static_cast<InterruptMode>(params[Controls::GenerateModeSwitches + stepIndex].getValue());
    }

  private:
    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
  };

  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType> class SustainStage {
    using Controls = CurveSequencerControls<N>;

  public:
    SustainStage(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                 std::vector<LightType> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    void setSustaining(int stepIndex, bool state) {
      lights[Controls::SustainingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
    }

    auto sustainMode(int stepIndex) -> InterruptMode {
      return static_cast<InterruptMode>(params[Controls::SustainModeSwitches + stepIndex].getValue());
    }

    auto execute(int step, Latch const &gateLatch) -> bool {
      auto const active = isActive(sustainMode(step), gateLatch);
      setSustaining(step, active);
      return active;
    }

  private:
    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
  };
} // namespace curve_sequencer
} // namespace dhe
