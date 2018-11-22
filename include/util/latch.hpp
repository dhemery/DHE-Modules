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
  void disable() { enabled_ = false; }

  /**
   * Resumes firing events.
   */
  void enable() { enabled_ = true; }

  /**
   * Registers an action to be called on each rising edge.
   * @param action called on each rising edge
   */
  template <typename Action> void on_rise(Action const &action) {
    rise_actions_.push_back(action);
  }

  /**
   * Registers an action to be called on each falling edge.
   * @param action called on each falling edge
   */
  template <typename Action> void on_fall(Action const &action) {
    fall_actions_.push_back(action);
  }

protected:
  enum class State { UNKNOWN, LOW, HIGH } state = State::UNKNOWN;

  void set_state(State incoming_state) {
    if (state == incoming_state)
      return;
    state = incoming_state;
    fire(state == State::HIGH ? rise_actions_ : fall_actions_);
  }

private:
  bool enabled_{true};
  std::vector<std::function<void()>> rise_actions_;
  std::vector<std::function<void()>> fall_actions_;

  void fire(std::vector<std::function<void()>> const &actions) const {
    if (!enabled_)
      return;
    for (auto const &action : actions)
      action();
  }
};
} // namespace DHE
