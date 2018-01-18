#include "Ramp.hpp"
#include "math.hpp"

namespace DHE {
Ramp::Ramp(std::function<float()> delta, std::function<void()> onEoC) { this->onEoC = onEoC; this->delta = delta; }
void Ramp::start() {
    value = 0.0;
    running = true;
}

void Ramp::stop() {
    value = 0.0;
    running = false;
}

void Ramp::step() {
    if (!running)
        return;
    value = rack::clampf(value + delta(), 0.0, 1.0);
    running = value < 1.0;
    if (!running)
        onEoC();
}

} // namespace DHE
