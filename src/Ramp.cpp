#include "Ramp.hpp"
#include "engine.hpp"
#include "math.hpp"

using namespace rack;

void Ramp::start() {
    value = 0.0;
    running = true;
}

void Ramp::stop() {
    value = 0.0;
    running = false;
}

void Ramp::step(float duration, PulseGenerator &eoc) {
    if (!running)
        return;
    float delta = 0.5 / (duration * engineGetSampleRate());
    value = clampf(value + delta, 0.0, 1.0);
    running = value < 1.0;
    if (!running) eoc.trigger(1e-3);
}
