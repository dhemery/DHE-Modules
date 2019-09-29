#pragma once

namespace dhe {
class Latch {
public:
  Latch(bool state = false, bool edge = false) : state{state}, changed{edge} {}

  void step(bool newState) {
    changed = state != newState;
    state = newState;
  }

  void set() {
    state = true;
    changed = false;
  }

  void reset() {
    state = false;
    changed = false;
  }

  auto high() -> bool { return state; }

  auto edge() -> bool { return changed; }

  auto rise() -> bool { return edge() && high(); }

private:
  bool state;
  bool changed;
};
} // namespace dhe