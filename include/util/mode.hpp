#pragma once

#include <functional>
#include <vector>

#include "d-flip-flop.hpp"

namespace DHE {
class Mode {
  std::vector<std::function<void()>> entry_actions;
  std::vector<std::function<void()>> exit_actions;
  std::vector<std::function<void()>> step_actions;

  void fire(std::vector<std::function<void()>> const &actions) {
    for (auto const &action : actions)
      action();
  }

public:
  void enter() { fire(entry_actions); }
  void exit() { fire(exit_actions); }
  virtual void step() { fire(step_actions); }

  template <typename Action> void on_entry(Action const &action) {
    entry_actions.push_back(action);
  }

  template <typename Action> void on_exit(Action const &action) {
    exit_actions.push_back(action);
  }

  template <typename Action> void on_step(Action const &action) {
    step_actions.push_back(action);
  }
};

class Modal {
  std::function<int()> const select;
  std::vector<Mode *> const modes;
  int current = -1;

public:
  template <typename Selector>
  Modal(Selector const &selector, std::vector<Mode *> modes)
      : select{selector}, modes{std::move(modes)} {}

  virtual void step() {
    auto const selected{select()};
    if (selected != current) {
      if (current >= 0) {
        modes[current]->exit();
      }
      current = selected;
      modes[current]->enter();
    }
    modes[current]->step();
  }
};

class CompoundMode : public Mode, Modal {
public:
  template <typename Selector>
  CompoundMode(Selector const &submode_selector,
               std::vector<Mode *> const &submodes)
      : Modal{submode_selector, submodes} {}

  void step() override {
    Modal::step();
    Mode::step();
  }
};
} // namespace DHE
