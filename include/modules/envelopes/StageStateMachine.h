#pragma once
#include "Generating.h"
#include "StateMachine.h"

#include <functional>
#include <utility>

namespace dhe {

class StageStateMachine : public StateMachine {
public:
  StageStateMachine(std::function<bool()> deferGateIsActive,
                    std::function<bool()> deferGateIsUp,
                    std::function<bool()> const &stageTriggerIsUp,
                    std::function<float()> duration,
                    std::function<void(float)> const &forward,
                    std::function<void()> prepare,
                    std::function<void(float)> generate,
                    std::function<void(bool)> const &setActive,
                    std::function<void(bool)> const &setEoc)
      : StateMachine{std::move(deferGateIsActive),
                     std::move(deferGateIsUp),
                     stageTriggerIsUp,
                     [this]() { enter(&generating); },
                     setActive,
                     setEoc,
                     forward},
        generating{[this]() { this->finishStage(); }, std::move(duration),
                   std::move(prepare), std::move(generate), setActive} {}

private:
  Generating generating;
};
} // namespace dhe
