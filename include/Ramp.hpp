#pragma once

#include <functional>
#include <utility>

namespace DHE {

    /**
     * A ramp whose phase progresses from 0 to 1. When the phase reaches 1,
     * further positive increments have no effect.
     */
    struct Ramp {
        /*!
         * Constructs a ramp that advances from 0 to 1 in increments supplied
         * by the supplier, and that calls onEndOfCycle() at the end of each
         * step that advances the phase to 1.
         *
         * A newly constructed ramp is stopped at phase 0.
         *
         * @param phaseIncrementSupplier called on each step to obtain the increment to advance the phase
         * @param onEndOfCycle called if a step advances the phase to 1
         */
        //
        Ramp(std::function<float()> phaseIncrementSupplier, std::function<void()> onEndOfCycle) :
                _onEndOfCycle(std::move(onEndOfCycle)),
                _phaseIncrement(std::move(phaseIncrementSupplier)) {
        }

        /**
         * Starts the ramp at phase 0.
         */
        void start() {
            _phase = 0.0;
            _running = true;
        }

        /*!
         * Stops the ramp at phase 0.
         */
        void stop() {
            _phase = 0.0;
            _running = false;
        }

        /**
         * Advances the phase by the supplied increment, to a maximum phase of 1.
         * If the phase advances to 1, the ramp stops running with phase == 1 and
         * calls onEndOfCycle(). If the ramp is not running, this function has no
         * effect.
         */
        void step();

        bool isRunning() const { return _running; }

        float phase() const { return _phase; }

    private:
        std::function<void()> _onEndOfCycle;
        std::function<float()> _phaseIncrement;
        bool _running = false;
        float _phase = 0.0f;
    };
} // namespace DHE
