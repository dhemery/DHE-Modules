#pragma once

#include <algorithm>
#include <cmath>
#include <array>

namespace dhe {
namespace scannibal {

template <template <int> class Module, typename Generator, int N>
class Controller {
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
  Controller(Module<N> &module, Generator &generator)
      : module_{module}, generator_{generator} {}

  void execute() {
    auto const position = scanner_position();
    auto const step = position.step();
    if (step != previous_step_) {
      if (previous_step_ >= 0) {
        module_.exit_step(previous_step_);
      }
      previous_step_ = step;
    }
    if (step >= 0) {
      module_.show_position(step, position.phase());
      generator_.generate(step, position.phase());
    }
  }

private:
  auto scanner_position() const -> Position {
    auto step_end_weight = std::array<float, N>{};
    auto step_weight = std::array<float, N>{};
    auto const length = module_.length();
    auto sequence_weight = 0.F;

    for (int i = 0; i < length; i++) {
      auto const weight = module_.duration(i);
      step_weight[i] = weight;
      sequence_weight += weight;
      step_end_weight[i] = sequence_weight;
    }

    if (sequence_weight == 0.F) {
      // Do not execute if all weights are 0
      return Position{-1, 0.F};
    }
    auto const scanner_weight = sequence_weight * scanner_phase();

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

  auto scanner_phase() const -> float {
    auto const phase_voltage = module_.phase();
    auto const raw_phase = phase_voltage * 0.1F;
    auto const phase = raw_phase - std::trunc(raw_phase);
    if (phase_voltage >= 10.F && phase == 0.F) {
      // From 10V upward, multiples of 10V map to phase == 1, not phase == 0
      return 1.F;
    }
    if (phase < 0.F) {
      return phase + 1.F;
    }
    return phase;
  }

  Module<N> &module_;
  Generator &generator_;
  int previous_step_{-1};
};
} // namespace scannibal
} // namespace dhe
