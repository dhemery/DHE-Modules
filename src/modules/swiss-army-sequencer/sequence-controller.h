#pragma once

#include "components/latch.h"
#include "status.h"

namespace dhe {
namespace swiss_army_sequencer {

template <typename Module, typename StepSelector, typename StepController>
class SequenceController {
public:
  SequenceController(Module &module, StepSelector &step_selector,
                     StepController &step_controller)
      : module_{module}, step_selector_{step_selector}, step_controller_{
                                                            step_controller} {}

  void execute(float sample_time) {
    // Process the latches even if not running. This ensures that we detect and
    // react to edges that happen on the same sample when RUN rises.
    gate_latch_.clock(module_.gate());
    reset_latch_.clock(module_.is_reset());

    // Reset even if not running.
    if (reset_latch_.is_rise()) {
      become_idle();
    }

    if (!module_.is_running()) {
      return;
    }

    if (step_ < 0) {
      idle();
    }
    if (step_ >= 0) {
      generate(sample_time);
    }
  }

private:
  void become_idle() {
    if (step_ >= 0) {
      step_controller_.exit();
    }
    step_ = -1;
  }

  void idle() {
    if (reset_latch_.is_high()) {
      // If RESET is high while idle, copy the input voltage to the output port.
      module_.output(module_.input());
    }
    if (gate_latch_.is_rise()) {
      // Consume the edge, so that the first step doesn't immediately exit if it
      // is set to advance on gate change.
      gate_latch_.clock(gate_latch_.is_high());
      start_sequence();
    }
  }

  void generate(float sample_time) {
    auto const status = step_controller_.execute(gate_latch_, sample_time);
    if (status == StepStatus::Completed) {
      advance_sequence();
    }
  }

  void start_sequence() {
    step_ = step_selector_.first();
    if (step_ >= 0) {
      step_controller_.enter(step_);
    }
  }

  void advance_sequence() {
    step_ = step_selector_.successor(step_, module_.is_looping());
    if (step_ >= 0) {
      step_controller_.enter(step_);
    }
  }

  int step_{-1};
  Latch gate_latch_{};
  Latch reset_latch_{};
  Module &module_;
  StepSelector &step_selector_;
  StepController &step_controller_;
}; // namespace swiss_army_sequencer
} // namespace swiss_army_sequencer
} // namespace dhe
