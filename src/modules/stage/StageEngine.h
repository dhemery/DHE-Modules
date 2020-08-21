#pragma once

#include "Event.h"
#include "Mode.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Range.h"

#include <algorithm>

namespace dhe {
namespace stage {

template <typename Controls, typename DeferMode, typename InputMode,
          typename GenerateMode, typename LevelMode>
class StageEngine {
public:
  StageEngine(Controls &controls, DeferMode &deferMode, InputMode &inputMode,
              GenerateMode &generateMode, LevelMode &levelMode)
      : controls{controls}, deferMode{deferMode}, inputMode{inputMode},
        generateMode{generateMode}, levelMode{levelMode} {}

  void process(float sampleTime) {
    defer.clock(controls.defer());
    gate.clock(controls.gate() && !defer.is_high());

    auto const newMode = identifyMode();
    if (mode != newMode) {
      enter(newMode);
    }

    switch (mode) {
    case Mode::Defer:
      deferMode.execute();
      break;
    case Mode::Generate: {
      if (generateMode.execute(gate, sampleTime) == Event::Completed) {
        eocTimer.reset();
        enter(Mode::Level);
      }
    } break;
    case Mode::Input:
      inputMode.execute();
      break;
    case Mode::Level:
      levelMode.execute();
      break;
    default:
      break;
    }

    eocTimer.advance(sampleTime / 1e-3F);
    controls.showEoc(eocTimer.in_progress());
  }

private:
  auto identifyMode() -> Mode {
    if (defer.is_high()) {
      return Mode::Defer;
    }
    if (gate.is_rise()) {
      return Mode::Generate;
    }
    return defer.is_fall() ? Mode::Input : mode;
  }

  void enter(Mode newMode) {
    switch (mode) {
    case Mode::Defer:
      deferMode.exit();
      break;
    case Mode::Generate:
      generateMode.exit();
      break;
    case Mode::Input:
      inputMode.exit();
      break;
    case Mode::Level:
      levelMode.exit();
      break;
    default:
      break;
    }

    mode = newMode;

    switch (mode) {
    case Mode::Defer:
      deferMode.enter();
      break;
    case Mode::Generate:
      generateMode.enter();
      break;
    case Mode::Input:
      inputMode.enter();
      break;
    case Mode::Level:
      levelMode.enter();
      break;
    default:
      break;
    }
  }

  PhaseTimer eocTimer{1.F};
  Mode mode{Mode::Input};
  Latch defer{};
  Latch gate{};
  Controls &controls;
  DeferMode &deferMode;
  InputMode &inputMode;
  GenerateMode &generateMode;
  LevelMode &levelMode;
};
} // namespace stage
} // namespace dhe
