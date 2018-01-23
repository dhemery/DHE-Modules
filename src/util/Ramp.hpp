#pragma once

#include <functional>
#include <utility>
#include "DLatch.hpp"
#include "Functions.hpp"
#include "engine.hpp"

namespace DHE {

/**
 * A ramp that advances from 0 to 1 over a specified duration.
 *
 * On each step, the ramp's phase advances by the proportion of the duration
 * that has passed since the previous step.
 *
 * To determine the amount of time that has passed between steps, the ramp
 * calls rack::engineGetSampleTime().
 *
 * To advance the ramp according to the passage of real time, call its step()
 * exactly once per Rack audio frame.
 *
 * When the phase advances to 1, the ramp fires endOfCycle and stops advancing.
 */
struct Ramp {
    /*!
     * Constructs a ramp that advances from 0 to 1 over the duration obtained
     * from the duration supplier, and that fires onEndOfCycle when the phase
     * advances to 1.
     *
     * Each step advances the phase by the proportion of the current duration
     * that has passed since the previous step.
     *
     * A newly constructed ramp is stopped at phase 0.
     *
     * @param durationSupplier called on each step to obtain the current ramp duration
     */
    explicit Ramp(std::function<float()> durationSupplier)
            : duration{std::move(durationSupplier)} {
        stop();
    }

    /*!
     * Constructs a ramp that advances from 0 to 1 over the given duration
     * (in seconds), and that fires onEndOfCycle when the phase advances to 1.
     *
     * A newly constructed ramp is stopped at phase 0.
     *
     * @param duration the duration over which to advance the phase to 1
     */
    explicit Ramp(float duration) :  Ramp([=]() { return duration; }) {}

    /**
     * Starts the ramp at phase 0.
     */
    void start() {
        progress = 0.0;
        active.resumeFiring();
        active.set();
    }

    /*!
     * Stops the ramp at phase 0.
     */
    void stop() {
        progress = 0.0;
        active.suspendFiring();
        active.reset();
    }

    /**
     * Advances the phase by the amount that would advance from 0 to 1 over the duration
     * supplied by the duration supplier.
     *
     * If the phase advances to 1, the ramp stops running and fires endOfCycle.
     * If the ramp is not running, this function has no effect.
     */
    void step() {
        if (!isActive()) return;

        progress = clamp(progress + rack::engineGetSampleTime() / duration(), 0.0, 1.0);

        if (progress >= 1.0f) {
            active.reset();
        };
    }

    bool isActive() const { return active.isHigh(); }

    float phase() const { return progress; }

    /**
     * Registers an action to be called when the ramp phase advances to 1.
     * @param action called when the ramp phase advances to 1
     */
    void onEndOfCycle(const std::function<void()> &action) {
        active.onFallingEdge(action);
    }

private:
    float progress = 0.0f;
    DLatch active{};
    std::function<float()> duration;
};
} // namespace DHE
