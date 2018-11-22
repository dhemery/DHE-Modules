#pragma once

#include "util/d-flip-flop.hpp"
#include <functional>
#include <vector>

namespace DHE {
struct Mode {
  void enter() { fire(entry_actions_); }
  void exit() { fire(exit_actions_); }
  void step() { fire(step_actions_); }

  template <typename Action> void on_entry(Action const &action) {
    entry_actions_.push_back(action);
  }

  template <typename Action> void on_exit(Action const &action) {
    exit_actions_.push_back(action);
  }

  template <typename Action> void on_step(Action const &action) {
    step_actions_.push_back(action);
  }

private:
  void fire(std::vector<std::function<void()>> const &actions) {
    for (auto const &action : actions)
      action();
  }

  std::vector<std::function<void()>> entry_actions_;
  std::vector<std::function<void()>> exit_actions_;
  std::vector<std::function<void()>> step_actions_;
};

class SubmodeSwitch : public Mode {
  DFlipFlop selector_;
  Mode *submode_;

  void enter_submode(Mode *incoming_submode) {
    submode_->exit();
    submode_ = incoming_submode;
    submode_->enter();
  }

public:
  template <typename Selector>
  SubmodeSwitch(Selector const &selector, Mode *low_mode, Mode *high_mode)
      : selector_{selector}, submode_{low_mode} {
    selector_.on_rise([this, high_mode] { enter_submode(high_mode); });
    selector_.on_fall([this, low_mode] { enter_submode(low_mode); });

    on_entry([this] { submode_->enter(); });
    on_step([this] {
      selector_.step();
      submode_->step();
    });
    on_exit([this] { submode_->exit(); });
  }
};
} // namespace DHE
