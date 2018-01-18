#include "Ramp.hpp"
#include "math.hpp"

namespace DHE {
Ramp::Ramp(std::function<float()> delta, std::function<void()> eoc) {
    this->eoc = eoc;
    this->delta = delta;
    stop();
}

void Ramp::start() {
    _value = 0.0;
    _running = true;
}

void Ramp::stop() {
    _value = 0.0;
    _running = false;
}

void Ramp::step() {
    if (!isRunning())
        return;
    _value = rack::clampf(_value + delta(), 0.0, 1.0);
    _running = _value < 1.0;
    if (!isRunning())
        eoc();
}

bool Ramp::isRunning() { return _running; }

float Ramp::value() { return _value; }

} // namespace DHE
