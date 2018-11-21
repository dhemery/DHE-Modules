#pragma once

#include "util/d-flip-flop.hpp"
#include <functional>
#include <vector>

namespace DHE {
struct Mode {
  void enter() { fire(entry_actions); }
  void exit() { fire(exit_actions); }
  void step() { fire(step_actions); }

  template <typename Action> void on_entry(Action &&action) {
    entry_actions.emplace_back(std::forward<Action>(action));
  }

  template <typename Action> void on_exit(Action &&action) {
    exit_actions.emplace_back(std::forward<Action>(action));
  }

  template <typename Action> void on_step(Action &&action) {
    step_actions.emplace_back(std::forward<Action>(action));
  }

private:
  void fire(const std::vector<std::function<void()>> &actions) {
    for (const auto &action : actions)
      action();
  }

  std::vector<std::function<void()>> entry_actions;
  std::vector<std::function<void()>> exit_actions;
  std::vector<std::function<void()>> step_actions;
};

class SubmodeSwitch : public Mode {
  DFlipFlop selector;
  Mode *submode;

  void enter_submode(Mode *incoming_submode) {
    submode->exit();
    submode = incoming_submode;
    submode->enter();
  }

public:
  template <typename Selector>
  SubmodeSwitch(Selector selector, Mode *low_mode, Mode *high_mode)
      : selector{std::move(selector)}, submode{low_mode} {
    this->selector.on_rise([this, high_mode] { enter_submode(high_mode); });
    this->selector.on_fall([this, low_mode] { enter_submode(low_mode); });

    on_entry([this] { submode->enter(); });
    on_step([this] {
      this->selector.step();
      submode->step();
    });
    on_exit([this] { submode->exit(); });
  }
};
} // namespace DHE
