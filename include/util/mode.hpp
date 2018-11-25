#pragma once

#include <functional>
#include <vector>

#include "d-flip-flop.hpp"

namespace DHE {
class Mode {
public:
  void enter() { fire(entry_actions); }
  void exit() { fire(exit_actions); }
  virtual void step() { fire(step_actions); }

  template <typename Action> void on_entry(const Action &action) {
    entry_actions.push_back(action);
  }

  template <typename Action> void on_exit(const Action &action) {
    exit_actions.push_back(action);
  }

  template <typename Action> void on_step(const Action &action) {
    step_actions.push_back(action);
  }

private:
  std::vector<std::function<void()>> entry_actions;
  std::vector<std::function<void()>> exit_actions;
  std::vector<std::function<void()>> step_actions;

  void fire(const std::vector<std::function<void()>> &actions) {
    for (const auto &action : actions)
      action();
  }
};

class Modal {
public:
  template <typename Selector>
  Modal(const Selector &selector, const std::vector<Mode *> &modes)
      : select{selector}, modes{modes} {}

  virtual void step() {
    auto selected = select();
    if (selected != current) {
      if (current >= 0) {
        modes[current]->exit();
      }
      current = selected;
      modes[current]->enter();
    }
    modes[current]->step();
  }

private:
  const std::function<int()> select;
  const std::vector<Mode *> modes;
  int current = -1;
};

class CompoundMode : public Mode, Modal {
public:
  template <typename Selector>
  CompoundMode(const Selector &submode_selector,
               const std::vector<Mode *> &submodes)
      : Modal{submode_selector, submodes} {}

  void step() override {
    Modal::step();
    Mode::step();
  }
};
} // namespace DHE
