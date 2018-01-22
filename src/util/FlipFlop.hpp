#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace DHE {

    /**
     * A flip-flop that, when stepped, determines its state by comparing the
     * incoming signal to the specified thresholds.
     *
     * On each step, the flip-flop fires one event:
     * - Fires risingEdge if the state changed from LOW or UNKNOWN to HIGH.
     * - Fires fallingEdge if the state changed from HIGH or UNKNOWN to LOW.
     * - Fires noChange if the state did not change.
     */
    struct FlipFlop {
        enum State {
            UNKNOWN, LOW, HIGH
        };

        /**
         * Creates a flip-flop that compares the given signal to the given thresholds.
         *
         * @param signal called on each step to obtain the value to evaluate
         * @param lowThreshold signal value at or below which the flip-flop state is LOW
         * @param highThreshold signal value at or above which the flip-flop state is HIGH
         */
        FlipFlop(std::function<float()> signal, float lowThreshold, float highThreshold) : signal(std::move(signal)),
                                                                                           lowThreshold(lowThreshold),
                                                                                           highThreshold(
                                                                                                   highThreshold) {}

        /**
         * Creates a flip-flop that compares the given signal to a low threshold of 0
         * and a high threshold of 1.
         *
         * @param signal called on each step to obtain the signal to evaluate
         */
        explicit FlipFlop(std::function<float()> signal) : FlipFlop(std::move(signal), 0.0f, 1.0f) {};

        /**
         * Sets the state by comparing the signal to the thresholds, and fires an event:
         * - Fires risingEdge if the state changed from LOW or UNKNOWN to HIGH.
         * - Fires fallingEdge if the state changed from HIGH or UNKNOWN to LOW.
         * - Fires noChange if the state did not change.
         */
        void step();

        bool isHigh() { return state == HIGH; }

        bool isLow() { return state == LOW; }

        /**
         * Suspends firing events. The flip-flop continues to track state.
         */
        void pause() {
            running = false;
        }

        /**
         * Resumes firing events.
         */
        void resume() {
            running = true;
        }

        /**
         * Registers the action to be called on  each rising edge.
         * @param action called on each rising edge
         */
        void onRisingEdge(const std::function<void()> &action) {
            risingEdge.push_back(action);
        }

        /**
         * Registers the action to be called on each falling edge.
         * @param action called on each falling edge
         */
        void onFallingEdge(const std::function<void()> &action) {
            fallingEdge.push_back(action);
        }

        /**
         * Registers the action to be called on each step that does not change state.
         * @param action called on each step that does not change state
         */
        void onNoChange(const std::function<void()> &action) {
            noChange.push_back(action);
        }

    private:
        bool running = true;
        State state = UNKNOWN;
        std::function<float()> signal;
        float lowThreshold;
        float highThreshold;
        std::vector<std::function<void()>> risingEdge;
        std::vector<std::function<void()>> fallingEdge;
        std::vector<std::function<void()>> noChange;

        void fire(std::vector<std::function<void()>> &actions) {
            if (!running) return;
            for (auto &&action : actions) action();
        }
    };
} // namespace DHE
