#pragma once

#include <algorithm>
#include <vector>

namespace dhe {
namespace scannibal {

template <typename Module, typename Generator> class Controller {
  class Position {
  public:
    Position(int step, float phase) : step_{step}, phase_{phase} {}
    auto step() const -> int { return step_; }
    auto phase() const -> float { return phase_; }

  private:
    int step_;
    float phase_;
  };

public:
  Controller(Module &module, Generator &generator)
      : module_{module}, generator_{generator} {}

  void execute() {
    auto position = scanner_position();
    module_.show_position(position.step(), position.phase());
    generator_.generate(position.step(), position.phase());
  }

private:
  auto scanner_position() const -> Position {
    auto const scanner_phase = module_.phase() * 0.1F;

    auto const length = module_.length();
    auto step_end_weight = std::vector<float>{};
    auto step_weight = std::vector<float>{};
    auto sequence_weight = 0.F;

    for (int i = 0; i < length; i++) {
      auto const weight = module_.duration(i);
      step_weight.push_back(weight);
      sequence_weight += weight;
      step_end_weight.push_back(sequence_weight);
    }

    auto const scanner_weight = sequence_weight * scanner_phase;

    auto selected_step = 0;
    for (; selected_step < length; selected_step++) {
      if (step_weight[selected_step] == 0.F) {
        // Do not execute a step with 0 weight
        continue;
      }
      if (step_end_weight[selected_step] >= scanner_weight) {
        break;
      }
    }
    auto weight_within_step = scanner_weight;
    if (selected_step > 0) {
      weight_within_step -= step_end_weight[selected_step - 1];
    }
    auto const phase_within_step =
        weight_within_step / step_weight[selected_step];
    return Position{selected_step, phase_within_step};
  };

  Module &module_;
  Generator &generator_;
  int step_{0};
};
} // namespace scannibal
} // namespace dhe
