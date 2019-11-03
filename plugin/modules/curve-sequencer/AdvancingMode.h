#pragma once

#include "Mode.h"
#include "components/Latch.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace curve_sequencer {
  using rack::engine::Input;
  using rack::engine::Param;

  template <int N> class AdvancingMode : public Mode {
  public:
    AdvancingMode(std::vector<Input> &inputs, std::vector<Param> &params) : inputs{inputs}, params{params} {}
    auto isTerminal() const -> bool override { return false; }

    auto execute(dhe::Latch const & /*runLatch*/, dhe::Latch const & /*gateLatch*/, int step,
                 float /*sampleTime*/) const -> Successor override {
      return {ModeId::Generating, step};
    };

  private:
    std::vector<Input> &inputs;
    std::vector<Param> &params;
  };
}; // namespace curve_sequencer
} // namespace dhe
