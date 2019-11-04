#pragma once

#include "CurveSequencerControls.h"
#include "Mode.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Input;
  using rack::engine::Param;

  template <int N> class AdvancingMode {
  public:
    AdvancingMode(std::vector<Input> &inputs, std::vector<Param> &params) : inputs{inputs}, params{params} {}

    auto execute(int currentStep) const -> Successor {
      auto const selectionStart = this->selectionStart();
      auto const selectionLength = this->selectionLength();
      if (!isSelected(currentStep, selectionStart, selectionLength)) {
        return {Mode::Idle, currentStep};
      }

      auto const selectionStop = selectionStart + selectionLength;
      for (int index = currentStep; index < selectionStop; index++) {
        auto const step = index & stepMask;
        if (isEnabled(step)) {
          return {Mode::Generating, step};
        }
      }
      return {Mode::Idle, currentStep};
    };

  private:
    using Controls = CurveSequencerControls<N>;

    auto selectionStart() const -> int { return static_cast<int>(valueOf(params[Controls::StartKnob])); }
    auto selectionLength() const -> int { return static_cast<int>(valueOf(params[Controls::StepsKnob])); }
    auto isSelected(int candidate, int selectionStart, int selectionLength) const -> bool {
      auto const selectionEnd = (selectionStart + selectionLength - 1) & stepMask;
      if (selectionEnd >= selectionStart) {
        return candidate >= selectionStart && candidate <= selectionEnd;
      }
      // else the selection wraps from highest step to lowest, e.g. [5 6 7 0 1 2]
      return candidate >= selectionStart || candidate <= selectionEnd;
    }

    auto isEnabled(int step) const -> bool {
      return isPressed(params[Controls::EnabledButtons + step]) || isHigh(inputs[Controls::EnabledInputs + step]);
    }
    std::vector<Input> &inputs;
    std::vector<Param> &params;
    int const stepMask = N - 1;
  };
}; // namespace curve_sequencer
} // namespace dhe
