#pragma once

#include "CurveSequencerControls.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <array>
#include <string>

namespace dhe {
namespace curve_sequencer {
  static auto constexpr riseMode = 0;
  static auto constexpr fallMode = 1;
  static auto constexpr edgeMode = 2;
  static auto constexpr highMode = 3;
  static auto constexpr lowMode = 4;
  static auto constexpr skipMode = 5;
  static auto constexpr durationMode = 6;

  static auto constexpr generateModeCount = durationMode + 1;
  static auto constexpr generateDefaultMode = durationMode;
  static auto constexpr sustainModeCount = skipMode + 1;
  static auto constexpr sustainDefaultMode = skipMode;

  static auto const generateModeDescriptions = std::array<std::string, generateModeCount>{
      "Advance if gate rises",       "Advance if gate falls",  "Advance if gate changes",
      "Advance if gate is high",     "Advance if gate is low", "Skip curve",
      "Advance when curve completes"};

  static auto const sustainModeDescriptions = std::array<std::string, sustainModeCount>{
      "Sustain until gate rises",   "Sustain until gate falls",  "Sustain until gate changes",
      "Sustain until gate is high", "Sustain until gate is low", "Skip sustain"};

  static inline auto isActive(int mode, Latch const &gateLatch) -> bool {
    switch (mode) {
    case riseMode:
      return !gateLatch.isRise();
    case fallMode:
      return !gateLatch.isFall();
    case edgeMode:
      return !gateLatch.isEdge();
    case highMode:
      return !gateLatch.isHigh();
    case lowMode:
      return !gateLatch.isLow();
    case durationMode:
      return true;
    case skipMode:
    default:
      return false;
    }
  }

  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType>
  class GenerateStage {
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
      lights[CurveSequencerControls<N>::GeneratingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
    }

    auto generateMode(int stepIndex) const -> int {
      return positionOf(params[CurveSequencerControls<N>::GenerateModeSwitches + stepIndex]);
    }

  private:
    std::vector<InputType> &inputs;
    std::vector<OutputType> &outputs;
    std::vector<ParamType> &params;
    std::vector<LightType> &lights;
  };

  template <int N, typename InputType, typename OutputType, typename ParamType, typename LightType> class SustainStage {
  public:
    SustainStage(std::vector<InputType> &inputs, std::vector<OutputType> &outputs, std::vector<ParamType> &params,
                 std::vector<LightType> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    void setSustaining(int stepIndex, bool state) {
      lights[CurveSequencerControls<N>::SustainingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
    }

    auto sustainMode(int stepIndex) -> int {
      return positionOf(params[CurveSequencerControls<N>::SustainModeSwitches + stepIndex]);
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
