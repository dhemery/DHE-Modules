#pragma once

#include <functional>
#include <utility>

#include "StageState.h"
#include "StateMachine.h"

#include "Holding.h"
#include "Sustaining.h"

namespace DHE {

class HostageStateMachine : public StateMachine {
public:
  HostageStateMachine(const std::function<bool()> &defer_gate_connected,
                      const std::function<bool()> &defer_gate,
                      const std::function<bool()> &stage_gate,
                      std::function<bool()> is_sustain_mode,
                      std::function<float()> duration,
                      const std::function<float()> &sample_time,
                      const std::function<void()> &forward,
                      const std::function<void(bool)> &set_active,
                      const std::function<void(bool)> &set_eoc)
      : StateMachine{sample_time, defer_gate_connected, defer_gate,
                     stage_gate,  set_active,           set_eoc,
                     forward},
        is_sustain_mode{std::move(is_sustain_mode)}, holding{duration,
                                                             sample_time,
                                                             set_active,
                                                             forward,
                                                             [this]() {
                                                               enter(&holding);
                                                             },
                                                             [this]() {
                                                               finish_stage();
                                                             }},
        sustaining{set_active, forward, [this]() { finish_stage(); }} {}

protected:
  void start_generating() override {
    if (is_sustain_mode()) {
      this->enter(&sustaining);
    } else {
      this->enter(&holding);
    }
  }

private:
  const std::function<bool()> is_sustain_mode;
  Holding holding;
  Sustaining sustaining;
}; // namespace DHE
} // namespace DHE
