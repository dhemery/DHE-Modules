#pragma once

#include <functional>
#include <utility>
#include "DLatch.hpp"
#include "Functions.hpp"

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
     */
    //
    explicit Ramp(std::function<float()> phaseIncrementSupplier) :
            phaseIncrement(std::move(phaseIncrementSupplier)) {
        stop();
    }

    /**
     * Starts the ramp at phase 0.
     */
    void start() {
        progress = 0.0;
        running.resume();
        running.set();
    }

    /*!
     * Stops the ramp at phase 0.
     */
    void stop() {
        progress = 0.0;
        running.pause();
        running.reset();
    }

    /**
     * Advances the phase by the supplied increment to a maximum phase of 1.
     * If the phase advances to 1, the ramp stops running and fires endOfCycle.
     * If the ramp is not running, this function has no effect.
     */
    void step() {
        if (!isRunning()) return;

        progress = clamp(progress + phaseIncrement(), 0.0, 1.0);

        if (progress >= 1.0f) running.reset();
    }

    bool isRunning() const { return running.isHigh(); }

    float phase() const { return progress; }

    /**
     * Registers an action to be called when the ramp phase advances to 1.
     * @param action called when the ramp phase advances to 1
     */
    void onEndOfCycle(const std::function<void()> &action) {
        running.onFallingEdge(action);
    }

private:
    DLatch running{};
    std::function<float()> phaseIncrement;
    float progress = 0.0f;
};
} // namespace DHE
