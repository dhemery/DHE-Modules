#pragma once

#include "./status.h"
#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace sequencizer {

static auto constexpr pulse_duration = 9e-4F;

template <typename Controls, typename StepSelector, typename StepController>
class SequenceController {
public:
  SequenceController(Controls &controls, StepSelector &step_selector,
                     StepController &step_controller)
      : controls_{controls}, step_selector_{step_selector},
        step_controller_{step_controller} {}

  void execute(float sample_time) {
    // Process the latches even if not running. This ensures that we detect and
    // react to edges that happen on the same sample when RUN rises.
    gate_latch_.clock(controls_.gate());
    reset_latch_.clock(controls_.is_reset());
    show_events(sample_time);

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
  void start_sequence() {
    start_of_sequence_.reset();
    change_to_step(first());
  }

  void advance_sequence() {
    end_of_step_.reset();
    auto const next_step = successor();
    if (next_step < 0 && controls_.is_looping()) {
      start_sequence();
    } else {
      change_to_step(next_step);
    }
  }

  void become_idle() { change_to_step(-1); }

  void idle() {
    if (gate_latch_.is_rise()) {
      // Consume the edge, so that the first step doesn't immediately exit if it
      // is set to advance on gate change.
      gate_latch_.clock(gate_latch_.is_high());
      start_sequence();
    }
  }

  void generate(float sample_time) {
    auto const status = step_controller_.execute(gate_latch_, sample_time);
    if (status == StepStatus::Idle) {
      advance_sequence();
    } else {
      show_status(status);
    }
  }

  auto first() const -> int { return step_selector_.first(); }

  auto successor() -> int { return step_selector_.successor(step_); }

  void change_to_step(int step) {
    step_ = step;
    if (step_ >= 0) {
      step_controller_.enter(step_);
      show_status(StepStatus::Generating);
    } else {
      step_controller_.exit();
      show_status(StepStatus::Idle);
    }
  }

  void show_status(StepStatus status) {
    controls_.show_step_status(step_, status);
  }

  void show_events(float sample_time) {
    auto const pulse_delta = sample_time / pulse_duration;
    start_of_sequence_.advance(pulse_delta);
    controls_.show_sequence_event(start_of_sequence_.in_progress());
    end_of_step_.advance(pulse_delta);
    controls_.show_step_event(end_of_step_.in_progress());
  }

  int step_{-1};
  Latch gate_latch_{};
  Latch reset_latch_{};
  PhaseTimer start_of_sequence_{1.F};
  PhaseTimer end_of_step_{1.F};
  Controls &controls_;
  StepSelector &step_selector_;
  StepController &step_controller_;
};
} // namespace sequencizer
} // namespace dhe
