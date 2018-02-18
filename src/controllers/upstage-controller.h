#ifndef DHE_MODULES_CONTROLLERS_UPSTAGE_CONTROLLER_H
#define DHE_MODULES_CONTROLLERS_UPSTAGE_CONTROLLER_H

#include <algorithm>
#include <utility>

#include "util/interval.h"
#include "controllers/level-control.h"

namespace DHE {

template<typename TModule>
struct UpstageController {
  explicit UpstageController(TModule *module, LevelControl level)
      : module{module},
        level{std::move(level)} {}

  void step() {
    module->send_trigger(trigger_out_voltage());
    module->send_level(level());
  }

private:
  TModule *module;
  LevelControl level;

  float trigger_in() const { return module->trigger_in(); }

  bool is_sending_triggers() const { return module->wait_in() < 1.0f and not module->wait_button_is_pressed(); }
  float trigger_button_voltage() const { return UNIPOLAR_CV.scale(module->trigger_button_is_pressed()); }
  float trigger_out_voltage() const { return is_sending_triggers() ? trigger_voltage() : 0.0f; }
  float trigger_voltage() const { return std::max(trigger_in(), trigger_button_voltage()); }
};

}
#endif
