#include "FlipFlop.hpp"

namespace DHE {
    void FlipFlop::step() {
        State newState = state;

        if (signal() >= highThreshold) newState = HIGH;
        else if (signal() <= lowThreshold) newState = LOW;

        if (newState == state) fire(noChange);
        else if (newState == HIGH) fire(risingEdge);
        else fire(fallingEdge);

        state = newState;
    }
} // namespace DHE
