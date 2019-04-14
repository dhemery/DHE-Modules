#pragma once

#include <functional>
#include <utility>
#include "HoldGenerator.h"
#include "StageState.h"

namespace DHE {

class Holding : public StageState {
public:
  Holding(std::function<float()> duration,
          const std::function<float()> &sample_time,
          std::function<void(bool)> set_active, std::function<void()> forward,
          const std::function<void()> &on_stage_gate_rise,
          const std::function<void()> &on_stage_complete)
      : StageState{on_stage_gate_rise}, set_active{std::move(set_active)},
        forward{std::move(forward)}, generator{std::move(duration), sample_time,
                                               on_stage_complete} {}

  void enter() override {
    set_active(true);
    forward();
    generator.start();
  }
  void step() override { generator.step(); }

  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
  HoldGenerator generator;
};
}
