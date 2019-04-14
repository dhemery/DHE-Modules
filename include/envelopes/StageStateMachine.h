#pragma once

#include <functional>
#include <utility>

#include "StageGenerator.h"
#include "StateMachine.h"

#include "Generating.h"

namespace DHE {

class StageStateMachine : public StateMachine {
public:
  StageStateMachine(const std::function<bool()> &defer_is_active,
                    const std::function<bool()> &defer_is_up,
                    const std::function<bool()> &stage_is_up,
                    const std::function<float()> &duration,
                    std::function<float()> const &sample_time,
                    const std::function<void(bool)> &set_active,
                    const std::function<void(bool)> &set_eoc,
                    const std::function<void()> &prepare_to_generate,
                    const std::function<void(float)> &generate,
                    const std::function<void()> &forward)
      : StateMachine{sample_time, defer_is_active, defer_is_up, stage_is_up,
                     set_active,  set_eoc,         forward},
        generating{duration,
                   sample_time,
                   set_active,
                   prepare_to_generate,
                   generate,
                   [this]() { start_generating(); },
                   [this]() { this->finish_stage(); }} {}

protected:
  void start_generating() override { this->enter(&generating); };

private:
  Generating generating;
};
} // namespace DHE
