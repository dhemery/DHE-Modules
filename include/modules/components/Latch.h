#pragma once

namespace dhe {
class Latch {
public:
  Latch(bool state = false, bool edge = false) : state{state}, edge{edge} {}

  void step(bool newState) {
    edge = state != newState;
    state = newState;
  }

  void set() {
    state = true;
    edge = false;
  }

  void reset() {
    state = false;
    edge = false;
  }

  auto isHigh() -> bool { return state; }

  auto isEdge() -> bool { return edge; }

private:
  bool state;
  bool edge;
};
} // namespace dhe