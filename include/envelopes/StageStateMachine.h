#pragma once

#include <functional>
#include <utility>

#include "StageGenerator.h"
#include "StateMachine.h"

#include "Generating.h"

namespace DHE {

class StageStateMachine : public StateMachine {
public:
  StageStateMachine(const std::function<bool()> &defer_gate_connected,
                    const std::function<bool()> &defer_gate,
                    const std::function<bool()> &stage_trigger,
                    const std::function<float()> &duration,
                    std::function<float()> const &sample_time,
                    const std::function<void()> &forward,
                    const std::function<void()> &prepare,
                    const std::function<void(float)> &generate,
                    const std::function<void(bool)> &set_active,
                    const std::function<void(bool)> &set_eoc)
      : StateMachine{sample_time,   defer_gate_connected, defer_gate,
                     stage_trigger, set_active,           set_eoc,
                     forward},
        generating{duration,
                   sample_time,
                   set_active,
                   prepare,
                   generate,
                   [this]() { start_generating(); },
                   [this]() { this->finish_stage(); }} {}

protected:
  void start_generating() override { this->enter(&generating); };

private:
  Generating generating;
};
} // namespace DHE
