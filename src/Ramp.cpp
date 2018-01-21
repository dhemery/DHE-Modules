#include "Ramp.hpp"
#include "math.hpp"

namespace DHE {
    void Ramp::step() {
        if (!_running)
            return;
        _phase = rack::clampf(_phase + _phaseIncrement(), 0.0, 1.0);
        _running = _phase < 1.0;
        if (!_running)
            _onEndOfCycle();
    }
} // namespace DHE
