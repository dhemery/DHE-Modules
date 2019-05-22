#pragma once
#include <functional>
#include <utility>

namespace dhe {

class StageState {
public:
  explicit StageState(std::function<void()> on_stage_gate_rise,
                      std::function<void()> on_stage_gate_fall,
                      std::function<void()> on_entry,
                      std::function<void(float)> on_step)
      : on_stage_gate_rise{std::move(on_stage_gate_rise)},
        on_stage_gate_fall{std::move(on_stage_gate_fall)},
        enter{std::move(on_entry)}, step{std::move(on_step)} {}
  const std::function<void()> on_stage_gate_rise;
  const std::function<void()> on_stage_gate_fall;
  const std::function<void()> enter;
  const std::function<void(float)> step;
};
} // namespace dhe
