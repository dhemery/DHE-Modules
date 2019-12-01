#pragma once

#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <string>

namespace dhe {

template <typename P, int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(std::string const &name = "toggle-" + std::to_string(N)) {
    auto base = name + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(controlSvg<P>(base + std::to_string(position)));
    }
  }

  static constexpr auto size = N;
};
} // namespace dhe
