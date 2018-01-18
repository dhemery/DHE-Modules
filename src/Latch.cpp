#include "Latch.hpp"

namespace DHE {

Latch::Latch() : Latch(0.0, 1.0) {}

Latch::Latch(float low, float high) {
    _low = low;
    _high = high;
}

void Latch::reset() { _state = UNKNOWN; }

Latch::Event Latch::process(float in) {
    State newState = _state;
    if (in >= _high) {
        newState = HIGH;
    } else if (in <= _low) {
        newState = LOW;
    }
    if (newState == _state)
        return NOCHANGE;
    _state = newState;
    return _state == HIGH ? RISE : FALL;
}

void Latch::process(float in, std::function<void()> onRise) {
    process(in, onRise, []() {}, []() {});
}

void Latch::process(float in, std::function<void()> onRise,
                    std::function<void()> onFall) {
    process(in, onRise, onFall, []() {});
}

void Latch::process(float in, std::function<void()> onRise,
                    std::function<void()> onFall,
                    std::function<void()> onNoChange) {
    switch (process(in)) {
    case RISE:
        onRise();
    case FALL:
        onFall();
    default:
        onNoChange();
    }
}

bool Latch::isHigh() { return _state == HIGH; }

bool Latch::isLow() { return _state == LOW; }

bool Latch::isUnknownState() { return _state == UNKNOWN; }

Latch::State Latch::state() { return _state; }
} // namespace DHE
