#pragma once

#include <functional>
#include <vector>
#include "util/d-flip-flop.h"

namespace DHE {
struct Mode {
  void enter() { fire(entry_actions);  }
  void exit() { fire(exit_actions); }
  void step() { fire(step_actions); }

  void on_entry(std::function<void()> action) {
    entry_actions.push_back(std::move(action));
  }

  void on_exit(std::function<void()> action) {
    exit_actions.push_back(std::move(action));
  }

  void on_step(std::function<void()> action) {
    step_actions.push_back(std::move(action));
  }

private:
  void fire(const std::vector<std::function<void()>> &actions) {
    for(const auto &action : actions) action();
  }

  std::vector<std::function<void()>> entry_actions;
  std::vector<std::function<void()>> exit_actions;
  std::vector<std::function<void()>> step_actions;
};


class SwitchedMode : public Mode {
  DFlipFlop mode_switch;
  Mode *current_mode;

  void enter_mode(Mode *incoming_mode) {
    current_mode->exit();
    current_mode = incoming_mode;
    current_mode->enter();
  }

public:
  SwitchedMode(std::function<float()> switch_signal, Mode *low_mode, Mode *high_mode) :
      mode_switch{std::move(switch_signal)}, current_mode{low_mode}{
    mode_switch.on_rising_edge([this, high_mode] {
      enter_mode(high_mode);
    });
    mode_switch.on_falling_edge([this, low_mode] {
      enter_mode(low_mode);
    });

    on_entry([this] {
      current_mode->enter();
    });
    on_step([this] {
      mode_switch.step();
      current_mode->step();
    });
    on_exit([this] {
      current_mode->exit();
    });
  }
};
}