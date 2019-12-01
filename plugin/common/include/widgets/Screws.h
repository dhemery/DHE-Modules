#pragma once

#include "CommonWidgets.h"
#include "Dimensions.h"

#include <componentlibrary.hpp>
#include <helpers.hpp>
#include <random>

namespace dhe {

using NormalScrew = rack::componentlibrary::ScrewSilver;
using SpecialScrew = rack::componentlibrary::ScrewBlack;

template <typename Panel> void installScrews(Panel *panel) {
  auto constexpr screwDiameter = hp2mm(1.F);
  auto constexpr screwRadius = screwDiameter / 2.F;
  auto const panelWidth = hp2mm(Panel::hp);

  auto constexpr top = screwRadius;
  auto constexpr bottom = moduleHeight - screwRadius;

  auto const left = Panel::hp < 3 ? screwRadius : screwRadius + screwDiameter;
  auto const right = panelWidth - left;

  auto screwPositions = std::vector<rack::math::Vec>{{left, top}, {right, bottom}};
  if (Panel::hp > 4) {
    screwPositions.emplace_back(right, top);
    screwPositions.emplace_back(left, bottom);
  }

  std::shuffle(screwPositions.begin(), screwPositions.end(), std::mt19937(std::random_device()()));

  auto const positionOfSpecialScrew = screwPositions.back();
  installWidget<SpecialScrew>(panel, positionOfSpecialScrew);

  screwPositions.pop_back();

  for (auto const screwPosition : screwPositions) {
    installWidget<NormalScrew>(panel, screwPosition);
  }
}

} // namespace dhe
