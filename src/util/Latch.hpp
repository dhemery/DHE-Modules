#pragma once

#include <functional>
#include <vector>

namespace DHE {

    class Latch {

    public:
        bool isHigh() { return state == HIGH; }

        bool isLow() { return state == LOW; }

        /**
         * Suspends firing events.
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
         * Registers the action to be called on each rising edge.
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
         * Registers the action to be called when the latch is set to its current state.
         * @param action called on when the latch is set to its current state
         */
        void onNoChange(const std::function<void()> &action) {
            noChange.push_back(action);
        }

    protected:
        enum State {
            UNKNOWN, LOW, HIGH
        } state = UNKNOWN;

        void setState(State newState) {
            if (state == newState) {
                fire(noChange);
            } else {
                state = newState;
                fire(state == HIGH ? risingEdge : fallingEdge);
            }
        }

    private:
        bool running = true;
        std::vector<std::function<void()>> risingEdge;
        std::vector<std::function<void()>> fallingEdge;
        std::vector<std::function<void()>> noChange;

        void fire(std::vector<std::function<void()>> &actions) {
            if (!running) return;
            for (auto &&action : actions) action();
        }
    };
}
