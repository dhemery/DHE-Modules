#pragma once

#include <app/SvgSwitch.hpp>
#include <string>

namespace dhe {

template <typename P, int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(const std::string &name = "toggle-" + std::to_string(N)) {
    auto base = name + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(P::svg(base + std::to_string(position)));
    }
  }

  static constexpr auto size = N;
};
} // namespace dhe
