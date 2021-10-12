#pragma once

#include "./event.h"
#include "./mode.h"
#include "components/latch.h"
#include "components/phase-timer.h"

#include <algorithm>

namespace dhe {
namespace stage {
template <typename Controls, typename InputMode, typename DeferMode,
          typename HoldMode, typename SustainMode, typename IdleMode>
class HostageEngine {
public:
  HostageEngine(Controls &controls, InputMode &input_mode,
                DeferMode &defer_mode, HoldMode &hold_mode,
                SustainMode &sustain_mode, IdleMode &idle_mode)
      : controls_{controls}, input_mode_{input_mode}, defer_mode_{defer_mode},
        hold_mode_{hold_mode}, sustain_mode_{sustain_mode}, idle_mode_{
                                                                idle_mode} {}

  void process(float sample_time) {
    defer_.clock(controls_.defer());
    gate_.clock(controls_.gate() && !defer_.is_high());

    auto const new_mode = identify_mode();
    if (mode_ != new_mode) {
      enter(new_mode);
    }

    switch (mode_) {
    case Mode::Defer:
      defer_mode_.execute();
      break;
    case Mode::Hold:
      if (hold_mode_.execute(gate_, sample_time) == Event::Completed) {
        eoc_timer_.reset();
        enter(Mode::Idle);
      }
      break;
    case Mode::Idle:
      idle_mode_.execute();
      break;
    case Mode::Sustain:
      if (sustain_mode_.execute(gate_) == Event::Completed) {
        eoc_timer_.reset();
        enter(Mode::Idle);
      }
      break;
    case Mode::Input:
      input_mode_.execute();
      break;
    default:
      break;
    }

    eoc_timer_.advance(sample_time / 1e-3F);
    controls_.show_eoc(eoc_timer_.in_progress());
  }

private:
  auto identify_mode() -> Mode {
    if (defer_.is_high()) {
      return Mode::Defer;
    }
    if (gate_.is_rise()) {
      return controls_.mode();
    }
    if (defer_.is_fall()) {
      if (controls_.mode() == Mode::Sustain) {
        eoc_timer_.reset();
        return Mode::Idle;
      }
      return Mode::Input;
    }
    return mode_;
  }

  void enter(Mode new_mode) {
    switch (mode_) {
    case Mode::Defer:
      defer_mode_.exit();
      break;
    case Mode::Hold:
      hold_mode_.exit();
      break;
    case Mode::Idle:
      idle_mode_.exit();
      break;
    case Mode::Input:
      input_mode_.exit();
      break;
    case Mode::Sustain:
      sustain_mode_.exit();
      break;
    default:
      break;
    }

    mode_ = new_mode;

    switch (mode_) {
    case Mode::Defer:
      defer_mode_.enter();
      break;
    case Mode::Hold:
      hold_mode_.enter();
      break;
    case Mode::Idle:
      idle_mode_.enter();
      break;
    case Mode::Sustain:
      sustain_mode_.enter();
      break;
    case Mode::Input:
      input_mode_.enter();
      break;
    default:
      break;
    }
  }

  Mode mode_{Mode::Input};
  PhaseTimer eoc_timer_{1.F};
  Latch defer_{};
  Latch gate_{};
  Controls &controls_;
  InputMode &input_mode_;
  DeferMode &defer_mode_;
  HoldMode &hold_mode_;
  SustainMode &sustain_mode_;
  IdleMode &idle_mode_;
}; // namespace stage
} // namespace stage
} // namespace dhe
