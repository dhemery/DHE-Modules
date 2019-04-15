#pragma once

#include <functional>
#include <utility>

#include "StateMachine.h"

#include "Holding.h"
#include "Sustaining.h"

namespace DHE {

class HostageStateMachine : public StateMachine {
public:
  HostageStateMachine(std::function<bool()> defer_gate_is_active,
                      std::function<bool()> defer_gate_is_up,
                      std::function<bool()> const &stage_gate_is_up,
                      std::function<bool()> is_sustain_mode,
                      std::function<float()> duration,
                      std::function<float()> const &sample_time,
                      std::function<void()> const &forward,
                      std::function<void(bool)> const &set_active,
                      std::function<void(bool)> const &set_eoc)
      : StateMachine{sample_time,
                     std::move(defer_gate_is_active),
                     std::move(defer_gate_is_up),
                     stage_gate_is_up,
                     [this]() { start_generating(); },
                     set_active,
                     set_eoc,
                     forward},
        is_sustain_mode{std::move(is_sustain_mode)},
        holding{[this]() { finish_stage(); }, std::move(duration), sample_time,
                forward, set_active},
        sustaining{[this]() { finish_stage(); }, forward, set_active} {}

protected:
  void start_generating() {
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
};
} // namespace DHE
