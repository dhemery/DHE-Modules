#pragma once

#include <functional>
#include <vector>

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
}