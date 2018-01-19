#include "Ramp.hpp"
#include "math.hpp"

namespace DHE {
    Ramp::Ramp(const std::function<float()> &phaseIncrementSupplier, const std::function<void()> &onEndOfCycle) {
        _onEndOfCycle = onEndOfCycle;
        _phaseIncrement = phaseIncrementSupplier;
        stop();
    }

    void Ramp::start() {
        _phase = 0.0;
        _running = true;
    }

    void Ramp::stop() {
        _phase = 0.0;
        _running = false;
    }

    void Ramp::step() {
        if (!_running)
            return;
        _phase = rack::clampf(_phase + _phaseIncrement(), 0.0, 1.0);
        _running = _phase < 1.0;
        if (!_running)
            _onEndOfCycle();
    }

    bool Ramp::isRunning() { return _running; }

    float Ramp::phase() { return _phase; }

} // namespace DHE
