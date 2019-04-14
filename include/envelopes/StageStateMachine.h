#include <utility>

#include <utility>

#include <utility>

#pragma once

#include <functional>
#include <utility>

#include "StateMachine.h"
#include "Generating.h"

namespace DHE {

class StageStateMachine : public StateMachine {
public:
  StageStateMachine(std::function<bool()> defer_gate_connected,
                    std::function<bool()> defer_gate,
                    std::function<bool()> const &stage_trigger,
                    std::function<float()> duration,
                    std::function<float()> const &sample_time,
                    std::function<void()> const &forward,
                    std::function<void()> prepare,
                    std::function<void(float)> generate,
                    std::function<void(bool)> const &set_active,
                    std::function<void(bool)> const &set_eoc)
      : StateMachine{sample_time,
                     std::move(defer_gate_connected),
                     std::move(defer_gate),
                     stage_trigger,
                     set_active,
                     set_eoc,
                     forward},
        generating{[this]() { start_generating(); },
                   [this]() { this->finish_stage(); },
                   std::move(duration),
                   sample_time,
                   std::move(prepare),
                   std::move(generate),
                   set_active} {}

protected:
  void start_generating() override { this->enter(&generating); };

private:
  Generating generating;
};
} // namespace DHE
