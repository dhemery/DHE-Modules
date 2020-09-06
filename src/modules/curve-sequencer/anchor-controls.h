#pragma once
#include "anchor-mode.h"
#include "anchor-source.h"
#include "control-ids.h"
#include "controls/level-inputs.h"
#include <vector>

namespace dhe {
namespace curve_sequencer {
template <typename InputT, typename OutputT, typename ParamT, int N>
class AnchorControls {
  using Input = dhe::curve_sequencer::InputIds<N>;
  using Output = dhe::curve_sequencer::OutputIds;
  using Param = dhe::curve_sequencer::ParamIds<N>;

public:
  AnchorControls(std::vector<InputT> &inputs, std::vector<OutputT> &outputs,
                 std::vector<ParamT> &params, int mode_base, int source_base,
                 int level_base)
      : inputs_{inputs}, outputs_{outputs}, params_{params},
        mode_base_{mode_base}, source_base_{source_base}, level_base_{
                                                              level_base} {}

  auto input() const -> float { return inputs_[Input::In].getVoltage(); }

  auto level(int step) const -> float {
    return dhe::selectable_level(params_[level_base_ + step],
                                 params_[Param::LevelRange]);
  }

  auto mode(int step) const -> AnchorMode {
    return static_cast<AnchorMode>(params_[mode_base_ + step].getValue());
  }

  auto output() const -> float { return outputs_[Output::Out].getVoltage(); }

  auto source(int step) const -> AnchorSource {
    return static_cast<AnchorSource>(params_[source_base_ + step].getValue());
  }

private:
  std::vector<InputT> &inputs_{};
  std::vector<OutputT> &outputs_{};
  std::vector<ParamT> &params_{};
  int mode_base_{};
  int source_base_{};
  int level_base_{};
};
} // namespace curve_sequencer
} // namespace dhe
