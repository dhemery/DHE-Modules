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
   * - Fires risingEdge if the latch was LOW or in an unknown state.
   * - Fires noChange if the latch was already HIGH.
   */
  void set() { set_state(HIGH); }

  /**
   * Sets the latch LOW.
   * - Fires fallingEdge if the latch was HIGH or in an unknown state.
   * - Fires noChange if the latch was already LOW.
   */
  void reset() { set_state(LOW); }
};
}
#endif

