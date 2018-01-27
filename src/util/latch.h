#ifndef DHE_UTIL_LATCH_H
#define DHE_UTIL_LATCH_H

#include <functional>
#include <vector>

namespace DHE {

class Latch {

public:
  bool is_high() const { return state==HIGH; }

  bool is_low() const { return state==LOW; }

  /**
   * Suspends firing events.
   */
  void suspend_firing() {
    firing_events = false;
  }

  /**
   * Resumes firing events.
   */
  void resume_firing() {
    firing_events = true;
  }

  /**
   * Registers an action to be called on each rising edge.
   * @param action called on each rising edge
   */
  void on_rising_edge(const std::function<void()> &action) {
    rising_edge_actions.push_back(action);
  }

  /**
   * Registers an action to be called on each falling edge.
   * @param action called on each falling edge
   */
  void on_falling_edge(const std::function<void()> &action) {
    falling_edge_actions.push_back(action);
  }

  /**
   * Registers an action to be called when the latch is set to its current state.
   * @param action called on when the latch is set to its current state
   */
  void on_no_change(const std::function<void()> &action) {
    no_change_actions.push_back(action);
  }

protected:
  enum State {
    UNKNOWN, LOW, HIGH
  } state = UNKNOWN;

  void set_state(State newState) {
    if (state==newState) {
      fire(no_change_actions);
    } else {
      state = newState;
      fire(state==HIGH ? rising_edge_actions : falling_edge_actions);
    }
  }

private:
  bool firing_events = true;
  std::vector<std::function<void()>> rising_edge_actions;
  std::vector<std::function<void()>> falling_edge_actions;
  std::vector<std::function<void()>> no_change_actions;

  void fire(std::vector<std::function<void()>> &actions) {
    if (!firing_events)
      return;
    for (auto &&action : actions)
      action();
  }
};
}
#endif
