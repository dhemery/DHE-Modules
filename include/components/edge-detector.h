#pragma once

namespace DHE {

/**
 * Tracks a boolean value and generates an event whenever the value changes.
 */
class EdgeDetector {
public:
  /**
   * Retrieves the current state of the tracked value and generates an event
   * if the state changes.
   */
  void step() {
    auto old_state = state;
    state = state_in();
    if (state != old_state) {
      on_state_change(state);
    }
  }

protected:
  /**
   * Retrieves the current state of the tracked value.
   */
  virtual auto state_in() const -> bool = 0;

  /**
   * The event generated whenever the tracked value changes.
   * @param state the new value of the tracked value
   */
  virtual void on_state_change(bool state) = 0;

private:
  bool state = false;
};
} // namespace DHE
