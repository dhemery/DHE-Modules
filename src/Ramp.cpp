#include "Ramp.hpp"
#include "engine.hpp"
#include "math.hpp"

using namespace rack;

void Ramp::start() {
    reset();
    running = true;
}

void Ramp::reset() { value = 0.0; }

void Ramp::step(float duration) {
    if (!running)
        return;
    float delta = 0.5 / (duration * engineGetSampleRate());
    value = clampf(value + delta, 0.0, 1.0);
    running = value < 1.0;
}
