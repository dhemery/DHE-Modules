#pragma once
#include <functional>
#include <utility>

#include "Generating.h"
#include "StateMachine.h"

namespace dhe {

class StageStateMachine : public StateMachine {
public:
  StageStateMachine(std::function<bool()> defer_gate_is_active,
                    std::function<bool()> defer_gate_is_up,
                    std::function<bool()> const &stage_trigger_is_up,
                    std::function<float()> duration,
                    std::function<void(float)> const &forward,
                    std::function<void()> prepare,
                    std::function<void(float)> generate,
                    std::function<void(bool)> const &set_active,
                    std::function<void(bool)> const &set_eoc)
      : StateMachine{std::move(defer_gate_is_active),
                     std::move(defer_gate_is_up),
                     stage_trigger_is_up,
                     [this]() { enter(&generating); },
                     set_active,
                     set_eoc,
                     forward},
        generating{[this]() { this->finish_stage(); }, std::move(duration),
                   std::move(prepare), std::move(generate), set_active} {}

private:
  Generating generating;
};
} // namespace dhe
