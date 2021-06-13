#pragma once

#include "components/latch.h"
#include "step-event.h"

namespace dhe {
namespace curve_sequencer {

template <typename Controls, typename StepSelector, typename StepController>
class CurveSequencer {
public:
  CurveSequencer(Controls &controls, StepSelector &step_selector,
                 StepController &step_controller)
      : controls_{controls}, step_selector_{step_selector},
        step_controller_{step_controller} {}

  void execute(float sample_time) {
    // Process the latches even if not running. This ensures that we detect and
    // react to edges that happen on the same sample when RUN rises.
    gate_latch_.clock(controls_.is_gated());
    reset_latch_.clock(controls_.is_reset());

    // Reset even if not running.
    if (reset_latch_.is_rise()) {
      become_idle();
    }

    if (!controls_.is_running()) {
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
      controls_.output(controls_.input());
    }
    if (gate_latch_.is_rise()) {
      // Consume the edge, so that the first step doesn't immediately exit if it
      // is set to advance on gate change.
      gate_latch_.clock(gate_latch_.is_high());
      start_sequence();
    }
  }

  void generate(float sample_time) {
    auto const result = step_controller_.execute(gate_latch_, sample_time);
    if (result == StepEvent::Completed) {
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
    step_ = step_selector_.successor(step_);
    if (step_ >= 0) {
      step_controller_.enter(step_);
    }
  }

  int step_{-1};
  Latch gate_latch_{};
  Latch reset_latch_{};
  Controls &controls_;
  StepSelector &step_selector_;
  StepController &step_controller_;
}; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe
