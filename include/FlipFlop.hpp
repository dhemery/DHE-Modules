#pragma once

#include <functional>
#include <memory>
#include <utility>

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
         * @param onRise called if a step yields a rising edge
         * @param onFall called if a step yields a falling edge
         * @param onNoChange called if a step yields no change in state
         */
        FlipFlop(std::function<float()> signal, float lowThreshold, float highThreshold,
                 std::function<void()> onRise, std::function<void()> onFall,
                 std::function<void()> onNoChange) :
                signal(std::move(signal)),
                lowThreshold(lowThreshold),
                highThreshold(highThreshold),
                onRise(std::move(onRise)),
                onFall(std::move(onFall)),
                onNoChange(std::move(onNoChange)) {}

        /**
         * Creates a flip-flop with the given reaction functions, a low
         * threshold of 0.0f, and a high threshold of 1.0f.
         *
         * @param signal called on each step to obtain the signal to evaluate
         * @param onRise called if a step yields a rising edge
         * @param onFall called if a step yields a falling edge
         * @param onNoChange called if a step yields no change in state
         */
        explicit FlipFlop(const std::function<float()> &signal, const std::function<void()> &onRise = []() {},
                          const std::function<void()> &onFall = []() {},
                          const std::function<void()> &onNoChange = []() {})
                : FlipFlop(signal, 0.0f, 1.0f, onRise, onFall, onNoChange) {}

        /**
         * Sets the state by comparing the signal to the thresholds
         * and calls the appropriate event function.
         * Fires onRise() if the step yields a rising edge.
         * Fires onFall() if the step yields a falling edge.
         * Fires onNoChange() if the step yields no change in state.
         */
        void step();

        bool isHigh() { return state == HIGH; }

        bool isLow() { return state == LOW; }

    private:
        State state = UNKNOWN;
        std::function<float()> signal;
        float lowThreshold;
        float highThreshold;
        std::function<void()> onRise;
        std::function<void()> onFall;
        std::function<void()> onNoChange;
    };
} // namespace DHE
