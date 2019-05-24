#pragma once

#include "Holding.h"
#include "StateMachine.h"
#include "Sustaining.h"

#include <functional>
#include <utility>

namespace dhe {

class HostageStateMachine : public StateMachine {
public:
  HostageStateMachine(std::function<bool()> deferGateIsActive, std::function<bool()> deferGateIsUp,
                      std::function<bool()> const &stageGateIsUp, std::function<bool()> isSustainMode,
                      std::function<float()> duration, std::function<void(float)> const &forward,
                      std::function<void(bool)> const &setActive, std::function<void(bool)> const &setEoc) :
      StateMachine{std::move(deferGateIsActive),
                   std::move(deferGateIsUp),
                   stageGateIsUp,
                   [this]() { startGenerating(); },
                   setActive,
                   setEoc,
                   forward},
      isSustainMode{std::move(isSustainMode)},
      holding{[this]() { finishStage(); }, std::move(duration), forward, setActive},
      sustaining{[this]() { finishStage(); }, forward, setActive} {}

protected:
  void startGenerating() {
    if (isSustainMode()) {
      this->enter(&sustaining);
    } else {
      this->enter(&holding);
    }
  }

private:
  const std::function<bool()> isSustainMode;
  Holding holding;
  Sustaining sustaining;
};
} // namespace dhe
