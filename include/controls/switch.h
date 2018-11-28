#pragma once

#include <functional>

#include <engine.hpp>

namespace DHE {

template <typename T> class Switch2 {
public:
  Switch2(const rack::Module *module, int index, T low, T high)
      : switch_param{&module->params[index]}, low{low}, high{high} {}

  auto operator()() const -> T {
    return switch_param->value > 0.5f ? high : low;
  }

private:
  const rack::Param *switch_param;
  T low;
  T high;
};

template <typename T> class Switch3 {
public:
  Switch3(const rack::Module *module, int index, T low, T mid, T high)
      : switch_param{&module->params[index]}, low{low}, mid{mid}, high{high} {}

  auto operator()() const -> T {
    auto position = switch_param->value;
    if (position > 1.5f)
      return high;
    if (position < 0.5f)
      return low;
    return mid;
  }

private:
  const rack::Param *switch_param;
  T low;
  T mid;
  T high;
};

} // namespace DHE
