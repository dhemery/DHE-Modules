#pragma once

#include <functional>

#include <engine.hpp>

namespace DHE {

class Button {
public:
  Button(const rack::Module *module, int index)
      : position{&module->params[index].value} {}

  inline auto operator()() const -> bool { return *position > 0.5f; }

private:
  const float *position;
};

template <typename T> class Switch2 {
public:
  Switch2(const rack::Module *module, int index, T low, T high)
      : position{&module->params[index].value}, low{low}, high{high} {}

  inline auto operator()() const -> T { return *position > 0.5f ? high : low; }

private:
  const float *position;
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
