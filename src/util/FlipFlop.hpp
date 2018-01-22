#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace DHE {

    /**
     * A flip-flop that, when stepped, determines its state by comparing the
     * incoming signal to the specified thresholds, and calls one of the
     * specified event functions depending on whether and how the state changes.
     */
    struct FlipFlop {
        enum State {
            UNKNOWN, LOW, HIGH
        };

        /**
         * Creates a flip-flop that with the given thresholds and reaction functions.
         *
         * @param signal called on each step to obtain the signal to evaluate
         * @param lowThreshold signal value at or below which the flip-flop is set LOW
         * @param highThreshold signal value at or above which the flip-flop is set HIGH
         */
        FlipFlop(std::function<float()> signal, float lowThreshold, float highThreshold) :
                signal(std::move(signal)),
                lowThreshold(lowThreshold),
                highThreshold(highThreshold){}

        FlipFlop(std::function<float()> signal) : FlipFlop(std::move(signal), 0.0f, 1.0f) {};

        /**
         * Sets the state by comparing the signal to the thresholds
         * and calls the appropriate event function.
         * Fires onRisingEdge() if the step yields a rising edge.
         * Fires onFallingEdge() if the step yields a falling edge.
         * Fires onNoChange() if the step yields no change in state.
         */
        void step();

        bool isHigh() { return state == HIGH; }

        bool isLow() { return state == LOW; }

        void onRisingEdge(const std::function<void()> &action) {
            risingEdge.push_back(action);
        }

        void onFallingEdge(const std::function<void()> &action) {
            fallingEdge.push_back(action);
        }

        void onNoChange(const std::function<void()> &action) {
            noChange.push_back(action);
        }

    private:
        State state = UNKNOWN;
        std::function<float()> signal;
        float lowThreshold;
        float highThreshold;
        std::vector<std::function<void()>> risingEdge;
        std::vector<std::function<void()>> fallingEdge;
        std::vector<std::function<void()>> noChange;

        void fire(std::vector<std::function<void()>> &actions) {
            for(auto &&action : actions) action();
        }
    };
} // namespace DHE
