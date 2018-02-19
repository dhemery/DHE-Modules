#pragma once

#include <functional>
#include <vector>

namespace DHE {

class Latch {

public:
  bool is_high() const { return state==State::HIGH; }

  bool is_low() const { return state==State::LOW; }

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
  void on_rising_edge(std::function<void()> action) {
    rising_edge_actions.push_back(std::move(action));
  }

  /**
   * Registers an action to be called on each falling edge.
   * @param action called on each falling edge
   */
  void on_falling_edge(std::function<void()> action) {
    falling_edge_actions.push_back(std::move(action));
  }

  /**
   * Registers an action to be called when the latch is set to its current state.
   * @param action called on when the latch is set to its current state
   */
  void on_no_change(std::function<void()> action) {
    no_change_actions.push_back(std::move(action));
  }

protected:
  enum class State {
    UNKNOWN, LOW, HIGH
  } state = State::UNKNOWN;

  void set_state(State newState) {
    if (state==newState) {
      fire(no_change_actions);
    } else {
      state = newState;
      fire(state==State::HIGH ? rising_edge_actions : falling_edge_actions);
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
    for (const auto &action : actions)
      action();
  }
};
}