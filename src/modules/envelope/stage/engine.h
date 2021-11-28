#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"
#include "modules/envelope/mode/events.h"
#include "modules/envelope/mode/mode-ids.h"

#include <algorithm>

namespace dhe {
namespace envelope {
namespace stage {

template <typename Signals, typename DeferMode, typename InputMode,
          typename GenerateMode, typename LevelMode>
struct Engine {
  Engine(Signals &signals, DeferMode &defer_mode, InputMode &input_mode,
         GenerateMode &generate_mode, LevelMode &level_mode)
      : signals_{signals}, defer_mode_{defer_mode}, input_mode_{input_mode},
        generate_mode_{generate_mode}, level_mode_{level_mode} {}

  void process(float sample_time) {
    defer_.clock(signals_.defer());
    gate_.clock(signals_.gate() && !defer_.is_high());

    auto const new_mode_id = identify_mode();
    if (mode_id_ != new_mode_id) {
      enter(new_mode_id);
    }

    switch (mode_id_) {
    case ModeId::Defer:
      defer_mode_.execute();
      break;
    case ModeId::Generate: {
      if (generate_mode_.execute(gate_, sample_time) == Event::Completed) {
        eoc_timer_.reset();
        enter(ModeId::Level);
      }
    } break;
    case ModeId::Input:
      input_mode_.execute();
      break;
    case ModeId::Level:
      level_mode_.execute();
      break;
    default:
      break;
    }

    eoc_timer_.advance(sample_time / 1e-3F);
    signals_.show_eoc(eoc_timer_.in_progress());
  }

private:
  auto identify_mode() -> ModeId {
    if (defer_.is_high()) {
      return ModeId::Defer;
    }
    if (gate_.is_rise()) {
      return ModeId::Generate;
    }
    return defer_.is_fall() ? ModeId::Input : mode_id_;
  }

  void enter(ModeId incoming_mode_id) {
    switch (mode_id_) {
    case ModeId::Defer:
      defer_mode_.exit();
      break;
    case ModeId::Generate:
      generate_mode_.exit();
      break;
    case ModeId::Input:
      input_mode_.exit();
      break;
    case ModeId::Level:
      level_mode_.exit();
      break;
    default:
      break;
    }

    mode_id_ = incoming_mode_id;

    switch (mode_id_) {
    case ModeId::Defer:
      defer_mode_.enter();
      break;
    case ModeId::Generate:
      generate_mode_.enter();
      break;
    case ModeId::Input:
      input_mode_.enter();
      break;
    case ModeId::Level:
      level_mode_.enter();
      break;
    default:
      break;
    }
  }

  PhaseTimer eoc_timer_{1.F};
  ModeId mode_id_{ModeId::Input};
  Latch defer_{};
  Latch gate_{};
  Signals &signals_;
  DeferMode &defer_mode_;
  InputMode &input_mode_;
  GenerateMode &generate_mode_;
  LevelMode &level_mode_;
};
} // namespace stage
} // namespace envelope
} // namespace dhe
