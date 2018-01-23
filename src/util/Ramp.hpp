#pragma once

#include <functional>
#include <utility>
#include "DLatch.hpp"
#include "Functions.hpp"
#include "engine.hpp"

namespace DHE {

/**
 * A ramp that advances its phase from 0 to 1 over time. When the phase reaches 1,
 * further steps have no effect.
 */
struct Ramp {
    /*!
     * Constructs a ramp that advances from 0 to 1 over the duration obtained
     * from the duration supplier, and that fires onEndOfCycle when the phase
     * advances to 1.
     *
     * Each step advances the phase as if the current duration were constant.
     *
     * A newly constructed ramp is stopped at phase 0.
     *
     * @param durationSupplier called on each step to obtain the ramp duration
     */
    explicit Ramp(std::function<float()> durationSupplier)
            : duration(durationSupplier) {
        stop();
    }

    /*!
     * Constructs a ramp that advances from 0 to 1 over the given duration, and that
     * fires onEndOfCycle when the phase advances to 1.
     *
     * A newly constructed ramp is stopped at phase 0.
     *
     * @param duration the duration over which to advance the phase to 1
     */
    Ramp(float duration, std::function<float()> stepUnitIncrement) :
            Ramp([&]() { return duration; }) {
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

        progress = clamp(progress + rack::engineGetSampleTime() / duration(), 0.0, 1.0);

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
    float progress = 0.0f;
    DLatch running{};
    std::function<float()> duration;
};
} // namespace DHE
