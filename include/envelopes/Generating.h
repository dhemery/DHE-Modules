#pragma once

#include <functional>
#include <utility>

#include "StageState.h"
#include "components/phase-accumulator.h"

namespace DHE {

class Generating : public StageState {
public:
  Generating(std::function<void()> on_stage_gate_rise,
             std::function<void()> on_stage_complete,
             std::function<float()> duration,
             std::function<float()> sample_time, std::function<void()> prepare,
             std::function<void(float)> generate,
             std::function<void(bool)> set_active)
      : StageState{std::move(on_stage_gate_rise)},
        set_active{std::move(set_active)}, prepare{std::move(prepare)},
        generator{std::move(duration), std::move(sample_time), []() {},
                  std::move(generate), std::move(on_stage_complete)} {}

  void enter() override {
    set_active(true);
    prepare();
    generator.start();
  }
  void step() override { generator.step(); }

  const std::function<void(bool)> set_active;
  const std::function<void()> prepare;
  PhaseAccumulator generator;
};
} // namespace DHE
