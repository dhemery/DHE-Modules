#pragma once

struct Ramp {
    bool running = false;
    float value = 0.0;

    void start();
    void reset();
    void step(float duration);
};
