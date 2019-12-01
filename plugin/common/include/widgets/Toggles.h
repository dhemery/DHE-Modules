#pragma once

#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace dhe {

template <typename P, int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(std::string name = "toggle-" + std::to_string(N)) : toggleName{std::move(name)} {}

  void setGraphics(P *panel) {
    std::cerr << "Loading Toggle " << toggleName << " in panel " << P::moduleSlug << std::endl;
    auto const controlNamePrefix = toggleName + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(controlSvg<P>(controlNamePrefix + std::to_string(position)));
    }
  }

private:
  std::string const toggleName;
  static constexpr auto size = N;
};
} // namespace dhe
