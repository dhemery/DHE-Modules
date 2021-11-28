#pragma once

#include "mode-ids.h"

#include "components/latch.h"
#include "components/phase-timer.h"
#include "modules/envelope/mode/events.h"
#include "modules/envelope/mode/mode-ids.h"

#include <algorithm>

namespace dhe {
namespace envelope {
namespace hostage {
template <typename Controls, typename InputMode, typename DeferMode,
          typename HoldMode, typename SustainMode, typename IdleMode>
struct Engine {
  Engine(Controls &controls, InputMode &input_mode, DeferMode &defer_mode,
         HoldMode &hold_mode, SustainMode &sustain_mode, IdleMode &idle_mode)
      : controls_{controls}, input_mode_{input_mode}, defer_mode_{defer_mode},
        hold_mode_{hold_mode}, sustain_mode_{sustain_mode}, idle_mode_{
                                                                idle_mode} {}

  void process(float sample_time) {
    defer_.clock(controls_.defer());
    gate_.clock(controls_.gate() && !defer_.is_high());

    auto const new_mode_id = identify_mode();
    if (mode_id_ != new_mode_id) {
      enter(new_mode_id);
    }

    switch (mode_id_) {
    case envelope::ModeId::Defer:
      defer_mode_.execute();
      break;
    case envelope::ModeId::Hold:
      if (hold_mode_.execute(gate_, sample_time) == Event::Completed) {
        eoc_timer_.reset();
        enter(envelope::ModeId::Idle);
      }
      break;
    case envelope::ModeId::Idle:
      idle_mode_.execute();
      break;
    case envelope::ModeId::Sustain:
      if (sustain_mode_.execute(gate_) == Event::Completed) {
        eoc_timer_.reset();
        enter(envelope::ModeId::Idle);
      }
      break;
    case envelope::ModeId::Input:
      input_mode_.execute();
      break;
    default:
      break;
    }

    eoc_timer_.advance(sample_time / 1e-3F);
    controls_.show_eoc(eoc_timer_.in_progress());
  }

private:
  auto identify_mode() -> envelope::ModeId {
    if (defer_.is_high()) {
      return envelope::ModeId::Defer;
    }
    if (gate_.is_rise()) {
      return controls_.mode();
    }
    if (defer_.is_fall()) {
      if (controls_.mode() == envelope::ModeId::Sustain) {
        eoc_timer_.reset();
        return envelope::ModeId::Idle;
      }
      return envelope::ModeId::Input;
    }
    return mode_id_;
  }

  void enter(envelope::ModeId new_mode_id) {
    switch (mode_id_) {
    case envelope::ModeId::Defer:
      defer_mode_.exit();
      break;
    case envelope::ModeId::Hold:
      hold_mode_.exit();
      break;
    case envelope::ModeId::Idle:
      idle_mode_.exit();
      break;
    case envelope::ModeId::Input:
      input_mode_.exit();
      break;
    case envelope::ModeId::Sustain:
      sustain_mode_.exit();
      break;
    default:
      break;
    }

    mode_id_ = new_mode_id;

    switch (mode_id_) {
    case envelope::ModeId::Defer:
      defer_mode_.enter();
      break;
    case envelope::ModeId::Hold:
      hold_mode_.enter();
      break;
    case envelope::ModeId::Idle:
      idle_mode_.enter();
      break;
    case envelope::ModeId::Sustain:
      sustain_mode_.enter();
      break;
    case envelope::ModeId::Input:
      input_mode_.enter();
      break;
    default:
      break;
    }
  }

  envelope::ModeId mode_id_{envelope::ModeId::Input};
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
} // namespace hostage
} // namespace envelope
} // namespace dhe
