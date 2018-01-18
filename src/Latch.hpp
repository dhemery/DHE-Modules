#pragma once
#include <functional>

namespace DHE {
struct Latch {
    enum Event { NOCHANGE, RISE, FALL };
    enum State { UNKNOWN, LOW, HIGH };

    // Constructs a latch with a low threshold of 0,
    // a high threshold of 1, and initial state of UNKNOWN.
    Latch();

    // Constructs a latch with the given thresholds
    // and an initial state of UNKNOWN.
    Latch(float low, float high);

    // Resets the latch state to UNKNOWN.
    void reset();

    // Updates the latch and returns the resulting event.
    Event process(float value);

    // Updates the latch and calls the function if the latch
    // rose from UNKNOWN or LOW to HIGH.
    void process(float value, std::function<void()> onRise);

    // Updates the latch and calls one of the functions
    // if the relevant event occurred.
    void process(float value, std::function<void()> onRise,
                 std::function<void()> onFall);

    // Updates the latch and calls the function associated
    // with the resulting event.
    void process(float value, std::function<void()> onRise,
                 std::function<void()> onFall,
                 std::function<void()> onNoChange);

    bool isHigh();
    bool isLow();
    bool isUnknownState();
    State state();

  private:
    float _low;
    float _high;
    State _state = UNKNOWN;
};
} // namespace DHE
