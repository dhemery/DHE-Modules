#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "Latch.hpp"

namespace DHE {

    /**
     * A latch determines its state by comparing the incoming signal to the specified
     * thresholds.
     */
    struct DFlipFlop : public Latch {

        /**
         * Creates a flip-flop that compares the given signal to the given thresholds.
         *
         * @param signal called on each step to obtain the value to evaluate
         * @param lowThreshold signal value at or below which the flip-flop state is LOW
         * @param highThreshold signal value at or above which the flip-flop state is HIGH
         */
        DFlipFlop(std::function<float()> signal, float lowThreshold, float highThreshold) : signal(std::move(signal)),
                                                                                           lowThreshold(lowThreshold),
                                                                                           highThreshold(
                                                                                                   highThreshold) {}
        /**
         * Creates a flip-flop that compares the given signal to a low threshold of 0
         * and a high threshold of 1.
         *
         * @param signal called on each step to obtain the signal to evaluate
         */
        explicit DFlipFlop(std::function<float()> signal) : DFlipFlop(std::move(signal), 0.0f, 1.0f) {};

        /**
         * Sets the state by comparing the signal to the thresholds.
         * - Fires risingEdge if the state changed to HIGH.
         * - Fires fallingEdge if the state changed to LOW.
         * - Fires noChange if the state did not change.
         */
        void step() {
            if (signal() >= highThreshold) setState(HIGH);
            else if (signal() <= lowThreshold) setState(LOW);
            else setState(state);
        }

    private:
        std::function<float()> signal;
        float lowThreshold;
        float highThreshold;
    };
} // namespace DHE
