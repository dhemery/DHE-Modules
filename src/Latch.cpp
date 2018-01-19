#include <util.hpp>
#include "Latch.hpp"

namespace DHE {

    Latch::Latch(float low, float high) {
        _low = low;
        _high = high;
    }

    void Latch::reset() { _state = UNKNOWN; }

    void Latch::process(float in, const std::function<void()> &onRise,
                        const std::function<void()> &onFall,
                        const std::function<void()> &onNoChange
    ) {
        State newState = _state;

        if (in >= _high) newState = HIGH;
        else if (in <= _low) newState = LOW;

        if (newState == _state) onNoChange();
        else if (newState == HIGH) onRise();
        else onFall();

        _state = newState;
    }

    bool Latch::isHigh() { return _state == HIGH; }

    bool Latch::isLow() { return _state == LOW; }

    bool Latch::isUnknownState() { return _state == UNKNOWN; }

    Latch::State Latch::state() { return _state; }

} // namespace DHE
