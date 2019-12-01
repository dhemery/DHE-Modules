#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <string>
#include <utility>

namespace dhe {

template <int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(std::string name = "toggle-" + std::to_string(N)) : toggleName{std::move(name)} {}

  void setGraphics(std::string const &moduleSlug, float x, float y) {
    auto const controlNamePrefix = toggleName + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(controlSvg(moduleSlug, controlNamePrefix + std::to_string(position)));
    }
    positionCentered(this, x, y);
  }

private:
  std::string const toggleName;
  static constexpr auto size = N;
};
} // namespace dhe
