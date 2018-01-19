#include "Ramp.hpp"
#include "math.hpp"

namespace DHE {
    Ramp::Ramp(const std::function<float()> &step, const std::function<void()> &onEndOfCycle) {
        cycleEnded = onEndOfCycle;
        delta = step;
        stop();
    }

    void Ramp::start() {
        progress = 0.0;
        running = true;
    }

    void Ramp::stop() {
        progress = 0.0;
        running = false;
    }

    void Ramp::step() {
        if (!running)
            return;
        progress = rack::clampf(progress + delta(), 0.0, 1.0);
        running = progress < 1.0;
        if (!running)
            cycleEnded();
    }

    bool Ramp::isRunning() { return running; }

    float Ramp::value() { return progress; }

} // namespace DHE
