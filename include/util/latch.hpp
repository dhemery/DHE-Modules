#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace DHE {

class Latch {

public:
  auto is_high() const -> bool { return state == State::HIGH; }

  /**
   * Suspends firing events.
   */
  void disable() { enabled = false; }

  /**
   * Resumes firing events.
   */
  void enable() { enabled = true; }

  /**
   * Registers an action to be called on each rising edge.
   * @param action called on each rising edge
   */
  template<typename Action>
  void on_rise(Action&& action) {
    rise_actions.emplace_back(action);
  }

  /**
   * Registers an action to be called on each falling edge.
   * @param action called on each falling edge
   */
  template<typename Action>
  void on_fall(Action&& action) {
    fall_actions.emplace_back(action);
  }

protected:
  enum class State { UNKNOWN, LOW, HIGH } state = State::UNKNOWN;

  void set_state(State incoming_state) {
    if (state == incoming_state)
      return;
    state = incoming_state;
    fire(state == State::HIGH ? rise_actions : fall_actions);
  }

private:
  bool enabled = true;
  std::vector<std::function<void()>> rise_actions;
  std::vector<std::function<void()>> fall_actions;

  void fire(const std::vector<std::function<void()>> &actions) const {
    if (!enabled)
      return;
    for (const auto &action : actions)
      action();
  }
};
} // namespace DHE
