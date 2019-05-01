#pragma once
#include <functional>

#include "ui/MenuItem.hpp"
#include "event.hpp"

namespace DHE {

struct BooleanOption : rack::ui::MenuItem {
  template <typename Setter, typename Getter>
  BooleanOption(const std::string &name, const Setter &setter,
                const Getter &getter)
      : set{setter}, is_on{getter} {
    text = name;
  }

  void onAction(const rack::event::Action &e) override { set(!is_on()); }

  void step() override {
    rightText = is_on() ? "✔" : "";
      rack::ui::MenuItem::step();
  }

  const std::function<void(bool)> set;
  const std::function<bool()> is_on;
};
} // namespace DHE
