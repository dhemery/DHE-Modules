#pragma once

#include <functional>

#include <engine.hpp>

namespace DHE {

template <typename T> class Switch {
public:
  static auto two(const rack::Module *module, int index, T low, T high)
      -> Switch<T> {
    const auto *param = &module->params[index];
    return Switch<T>{
        [param, low, high] { return param->value > 0.5f ? high : low; }};
  }

  static auto three(const rack::Module *module, int index, T low, T mid, T high)
      -> Switch<T> {
    const auto *param = &module->params[index];
    return Switch<T>{[param, low, mid, high] {
      const auto selection = param->value;
      if (selection < 0.5f)
        return low;
      if (selection > 1.5f)
        return high;
      return mid;
    }};
  }

  auto operator()() const -> T { return select(); }

private:
  template <typename Selector>
  explicit Switch(Selector select) : select{select} {}

  const std::function<T()> select;
};
} // namespace DHE
