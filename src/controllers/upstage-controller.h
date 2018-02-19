#ifndef DHE_MODULES_CONTROLLERS_UPSTAGE_CONTROLLER_H
#define DHE_MODULES_CONTROLLERS_UPSTAGE_CONTROLLER_H

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
      OutputPortControl stage_out)
      : level{std::move(level)},
        wait_in{wait_in},
        trigger_in{trigger_in},
        trigger_out{trigger_out},
        stage_out{stage_out} {}

  void step() {
    trigger_out(wait_in() < 0.5 ? trigger_in() : 0.f);
    stage_out(level());
  }

private:
  LevelControl level;
  InputPortControl wait_in;
  InputPortControl trigger_in;
  OutputPortControl trigger_out;
  OutputPortControl stage_out;
};

}
#endif
