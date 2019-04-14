#pragma once

#include "StageGenerator.h"
#include "StageState.h"

namespace DHE {

class Generating : public StageState {
public:
  Generating(const std::function<float()> &duration,
             const std::function<float()> &sample_time,
             std::function<void(bool)> set_active,
             std::function<void()> prepare_to_generate,
             const std::function<void(float)> &generate,
             const std::function<void()> &on_stage_gate_rise,
             const std::function<void()> &on_stage_complete)
      : StageState{on_stage_gate_rise}, set_active{std::move(set_active)},
        prepare_to_generate{std::move(prepare_to_generate)},
        generator{duration, sample_time, generate, on_stage_complete} {}

  void enter() override {
    set_active(true);
    prepare_to_generate();
    generator.start();
  }
  void step() override { generator.step(); }

  const std::function<void(bool)> set_active;
  const std::function<void()> prepare_to_generate;
  StageGenerator generator;
};
} // namespace DHE
