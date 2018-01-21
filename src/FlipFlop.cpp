#include "FlipFlop.hpp"

namespace DHE {

    FlipFlop::FlipFlop(const std::function<float()> &signal,
                       float lowThreshold, float highThreshold,
                       const std::function<void()> &onRise,
                       const std::function<void()> &onFall,
                       const std::function<void()> &onNoChange
    ) {
        this->signal = signal;
        this->lowThreshold = lowThreshold;
        this->highThreshold = highThreshold;
        this->onRise = onRise;
        this->onFall = onFall;
        this->onNoChange = onNoChange;
    }

    void FlipFlop::step() {
        State newState = _state;

        if (signal() >= highThreshold) newState = HIGH;
        else if (signal() <= lowThreshold) newState = LOW;

        if (newState == _state) onNoChange();
        else if (newState == HIGH) onRise();
        else onFall();

        _state = newState;
    }

    bool FlipFlop::isHigh() { return _state == HIGH; }

    bool FlipFlop::isLow() { return _state == LOW; }
} // namespace DHE
