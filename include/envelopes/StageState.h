#pragma once

#include <functional>
#include <utility>

#include "components/mode.h"

namespace DHE {

class StageState : public Mode {
public:
  explicit StageState(std::function<void()> on_stage_gate_rise = []() {},
                      std::function<void()> on_stage_gate_fall = []() {})
      : on_stage_gate_rise{std::move(on_stage_gate_rise)},
        on_stage_gate_fall{std::move(on_stage_gate_fall)} {}

  const std::function<void()> on_stage_gate_rise;
  const std::function<void()> on_stage_gate_fall;
};
} // namespace DHE
