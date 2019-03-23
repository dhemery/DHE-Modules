#pragma once

#include "trigger.h"

namespace DHE {

/**
 * Tracks a boolean value and generates an event whenever the value changes.
 */
class Gate : public Trigger {
protected:
  /**
   * The event generated whenever the tracked value changes from false to true.
   */
  virtual void on_fall() = 0;

  void on_state_change(bool state) override {
    if (state) {
      on_rise();
    } else {
      on_fall();
    }
  }
};
} // namespace DHE
