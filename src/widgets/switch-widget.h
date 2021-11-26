#pragma once
#include "assets.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <typeinfo>
#include <vector>

namespace dhe {
template <typename T> struct SwitchWidget : public rack::app::SvgSwitch {
  using Value = typename T::ValueType;
  using Action = std::function<void(Value)>;

  SwitchWidget() {
    static auto const file_names = frame_file_names();
    auto const panel_prefix = std::string{T::svg_dir} + "/";
    for (auto const &file_name : file_names) {
      addFrame(load_svg(panel_prefix + file_name));
    }
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    DEBUG(">> Value %s", typeid(Value).name());
    rack::app::SvgSwitch::onChange(e);
    notify();
    DEBUG("<< Value %s", typeid(Value).name());
  }

  void on_change(Action const &action) {
    DEBUG(">> Value %s", typeid(Value).name());
    action_ = action;
    if (module != nullptr) {
      notify();
    }
    DEBUG("<< Value %s", typeid(Value).name());
  }

private:
  inline void notify() {
    DEBUG(">> Value %s", typeid(Value).name());
    auto const value = this->getParamQuantity()->getValue();
    auto const selection = static_cast<Value>(value);
    action_(selection);
    DEBUG("<< Value %s", typeid(Value).name());
  }

  static inline auto frame_file_names() -> std::vector<std::string> {
    auto names = std::vector<std::string>{};
    auto const prefix = T::slug + std::string{"-"};
    auto const size = T::size;
    for (size_t position = 1; position <= size; position++) {
      names.push_back(prefix + std::to_string(position));
    }
    return names;
  }

  Action action_ = [](Value /*unused*/) {};
};

} // namespace dhe
