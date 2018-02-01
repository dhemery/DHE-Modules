#ifndef DHE_UTIL_D_LATCH_H
#define DHE_UTIL_D_LATCH_H

#include <functional>
#include <vector>

#include "latch.h"

namespace DHE {

/**
 * A latch that can be set and reset.
 */
struct DLatch : Latch {
  /**
   * Sets the latch HIGH.
   * - Fires 'rising edge' if the latch was LOW or in an unknown state.
   * - Fires 'no change' if the latch was already HIGH.
   */
  void set() { set_state(State::HIGH); }

  /**
   * Sets the latch LOW.
   * - Fires 'falling edge' if the latch was HIGH or in an unknown state.
   * - Fires 'no change' if the latch was already LOW.
   */
  void reset() { set_state(State::LOW); }
};
}
#endif

