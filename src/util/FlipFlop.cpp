#include "FlipFlop.hpp"

namespace DHE {
    void FlipFlop::step() {
        State newState = state;

        if (signal() >= highThreshold) newState = HIGH;
        else if (signal() <= lowThreshold) newState = LOW;

        if (newState == state) onNoChange();
        else if (newState == HIGH) onRise();
        else onFall();

        state = newState;
    }
} // namespace DHE
