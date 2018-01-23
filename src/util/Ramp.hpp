#pragma once

#include <functional>
#include <utility>
#include "DLatch.hpp"
#include "Functions.hpp"
#include "engine.hpp"

namespace DHE {

/**
 * A ramp that advances its phase from 0 to 1 at a specified rate (in inverse
 * seconds, or Hertz).
 *
 * The ramp uses Rack's audio engine as a clock to determine the passage of
 * time. To advance the ramp at rates that match the passage of real time, call
 * the ramp's step() function exactly once per Rack audio frame.
 *
 * When the phase advances to 1, the ramp fires endOfCycle and stops advancing.
 */
struct Ramp {
    /*!
     * Constructs a ramp that calls the supplier on each step and advances the
     * phase at the resulting rate.
     *
     * The newly constructed ramp is at phase 0 and inactive.
     *
     * @param rateSupplier supplies the rate at which to advance the phase on each step
     */
    explicit Ramp(std::function<float()> rateSupplier)
            : rate{std::move(rateSupplier)} {
        stop();
    }

    /*!
     * Constructs a ramp that advances at a fixed rate (in inverse seconds, or
     * Hertz).
     *
     * The newly constructed ramp is at phase 0 and inactive.
     *
     * @param rate the rate (inverse seconds, or Hertz) at which to advance the phase
     */
    explicit Ramp(float rate) :  Ramp([=]() { return rate; }) {}

    /**
     * Activates the ramp at phase 0.
     *
     * Subsequent calls to step() will advance the phase.
     */
    void start() {
        progress = 0.0;
        active.resumeFiring();
        active.set();
    }

    /*!
     * Deactivates the ramp at phase 0.
     *
     * Subsequent calls to step() will not advance the phase.
     *
     * Stopping the ramp does not fire an endOfCycle event.
     */
    void stop() {
        progress = 0.0;
        active.suspendFiring();
        active.reset();
    }

    /**
     * Advances the phase at the ramp's current rate.
     *
     * If the phase advances to 1, the ramp fires endOfCycle and becomes
     * inactive.
     *
     * If the ramp is inactive, this function has no effect.
     */
    void step() {
        if (!isActive()) return;

        progress = clamp(progress + rate() * rack::engineGetSampleTime(), 0.0, 1.0);

        if (progress >= 1.0f) {
            active.reset();
        };
    }

    /**
     * Indicates whether the ramp is active.
     *
     * @return whether the ramp is active
     */
    bool isActive() const { return active.isHigh(); }

    /**
     * Returns the ramp's phase.
     *
     * If the ramp is active, the phase indicates the ramp's progress
     * from 0 to 1.
     *
     * If the ramp is inactive, the phase is 1 (if the ramp became inactive by
     * completing its advancement) or 0 (if the ramp was stopped).
     *
     * @return the ramp's phase
     */
    float phase() const { return progress; }

    /**
     * Registers an action to be called when the ramp's phase advances to 1.
     * @param action called when the ramp phase advances to 1
     */
    void onEndOfCycle(const std::function<void()> &action) {
        active.onFallingEdge(action);
    }

private:
    float progress = 0.0f;
    DLatch active{};
    std::function<float()> rate;
};
} // namespace DHE
