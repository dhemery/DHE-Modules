#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"
#include "modules/envelope/mode/event.h"
#include "modules/envelope/mode/mode.h"

#include <algorithm>

namespace dhe {
namespace envelope {
namespace stage {

template <typename Controls, typename DeferMode, typename InputMode,
          typename GenerateMode, typename LevelMode>
struct StageEngine {
  using Mode = mode::Mode;
  using Event = mode::Event;

  StageEngine(Controls &controls, DeferMode &defer_mode, InputMode &input_mode,
              GenerateMode &generate_mode, LevelMode &level_mode)
      : controls_{controls}, defer_mode_{defer_mode}, input_mode_{input_mode},
        generate_mode_{generate_mode}, level_mode_{level_mode} {}

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
    case Mode::Generate: {
      if (generate_mode_.execute(gate_, sample_time) == Event::Completed) {
        eoc_timer_.reset();
        enter(Mode::Level);
      }
    } break;
    case Mode::Input:
      input_mode_.execute();
      break;
    case Mode::Level:
      level_mode_.execute();
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
      return Mode::Generate;
    }
    return defer_.is_fall() ? Mode::Input : mode_;
  }

  void enter(Mode incoming_mode) {
    switch (mode_) {
    case Mode::Defer:
      defer_mode_.exit();
      break;
    case Mode::Generate:
      generate_mode_.exit();
      break;
    case Mode::Input:
      input_mode_.exit();
      break;
    case Mode::Level:
      level_mode_.exit();
      break;
    default:
      break;
    }

    mode_ = incoming_mode;

    switch (mode_) {
    case Mode::Defer:
      defer_mode_.enter();
      break;
    case Mode::Generate:
      generate_mode_.enter();
      break;
    case Mode::Input:
      input_mode_.enter();
      break;
    case Mode::Level:
      level_mode_.enter();
      break;
    default:
      break;
    }
  }

  PhaseTimer eoc_timer_{1.F};
  Mode mode_{Mode::Input};
  Latch defer_{};
  Latch gate_{};
  Controls &controls_;
  DeferMode &defer_mode_;
  InputMode &input_mode_;
  GenerateMode &generate_mode_;
  LevelMode &level_mode_;
};
} // namespace stage
} // namespace envelope
} // namespace dhe
