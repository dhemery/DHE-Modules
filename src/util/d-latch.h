#pragma once

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
    void set() { setState(HIGH); }

    /**
     * Sets the latch LOW.
     * - Fires fallingEdge if the latch was HIGH or in an unknown state.
     * - Fires noChange if the latch was already LOW.
     */
    void reset() { setState(LOW); }
};
}

