#pragma once

#include <algorithm>
#include <utility>

#include "util/interval.h"
#include "input-port-control.h"
#include "level-control.h"
#include "output-port-control.h"

namespace DHE {

struct UpstageController {
  explicit UpstageController(
      LevelControl level,
      InputPortControl wait_in,
      InputPortControl trigger_in,
      OutputPortControl trigger_out,
      std::function<void(float)> stage_out)
      : level{std::move(level)},
        wait_in{std::move(wait_in)},
        trigger_in{std::move(trigger_in)},
        trigger_out{std::move(trigger_out)},
        stage_out{std::move(stage_out)} {}

  void step() {
    trigger_out(wait_in() > 0.5f ? 0.f : trigger_in());
    trigger_in(); // To set the light
    stage_out(level());
  }

private:
  LevelControl level;
  InputPortControl wait_in;
  InputPortControl trigger_in;
  OutputPortControl trigger_out;
  std::function<void(float)> stage_out;
};

}