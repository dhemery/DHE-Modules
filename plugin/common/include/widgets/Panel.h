#pragma once

#include "Buttons.h"
#include "Dimensions.h"
#include "Jacks.h"
#include "Knobs.h"
#include "PanelAssets.h"
#include "Params.h"
#include "Toggles.h"

#include <algorithm>
#include <app.hpp>
#include <app/ModuleWidget.hpp>
#include <app/ParamWidget.hpp>
#include <app/PortWidget.hpp>
#include <app/SvgPanel.hpp>
#include <app/SvgPort.hpp>
#include <app/common.hpp>
#include <event.hpp>
#include <helpers.hpp>
#include <math.hpp>
#include <random>
#include <string>
#include <utility>
#include <widget/Widget.hpp>

namespace dhe {

template <typename P> class Panel : public rack::app::ModuleWidget {
private:
public:
  Panel(rack::engine::Module *module, int widgetHp) {
    setModule(module);
    box.size = rack::math::Vec{hp2px((float) widgetHp), rack::app::RACK_GRID_HEIGHT};

    auto panel = new rack::app::SvgPanel();
    panel->setBackground(panelSvg<P>());
    addChild(panel);

    installScrews(widgetHp);
  }

protected:
  auto height() const -> float { return box.size.y * rack::app::MM_PER_IN / rack::app::SVG_DPI; }

  auto width() const -> float { return box.size.x * rack::app::MM_PER_IN / rack::app::SVG_DPI; }

  template <template <typename> class K> auto knob(float x, float y, int index) -> K<P> * {
    return installParam<K<P>>(this, module, x, y, index);
  }

  template <template <typename> class B = Button> auto button(float x, float y, int index) -> B<P> * {
    return installParam<B<P>>(this, module, x, y, index);
  }

  template <typename L> void light(float x, float y, int index) {
    auto *light = rack::createLightCentered<rack::componentlibrary::SmallLight<L>>(mm2px(x, y), module, index);
    addChild(light);
  }

  template <template <typename> class C> auto toggle(float x, float y, int index) -> C<P> * {
    return installParam<C<P>>(this, module, x, y, index);
  }

  template <template <typename, int> class C, int N> auto toggle(float x, float y, int index) -> C<P, N> * {
    return installParam<C<P, N>>(this, module, x, y, index);
  }

  template <int N> auto toggle(float x, float y, int index) -> Toggle<P, N> * {
    return installParam<Toggle<P, N>>(this, module, x, y, index);
  }

  auto input(float x, float y, int index) -> InputJack<P> * { return InputJack<P>::install(this, module, x, y, index); }

  auto output(float x, float y, int index) -> OutputJack<P> * {
    return OutputJack<P>::install(this, module, x, y, index);
  }

private:
  template <typename T> void screw(rack::math::Vec pos) {
    addChild(rack::createWidgetCentered<T>(rack::app::mm2px(pos)));
  }

  void installScrews(int widgetHp) {
    auto screwDiameter = rack::app::RACK_GRID_WIDTH * rack::app::MM_PER_IN / rack::app::SVG_DPI;
    auto screwRadius = screwDiameter / 2.F;

    auto const top = screwRadius;
    auto const bottom = height() - top;

    auto const left = widgetHp < 3 ? screwRadius : screwRadius + screwDiameter;
    auto const right = width() - left;

    auto screwPositions = std::vector<rack::math::Vec>{{left, top}, {right, bottom}};
    if (widgetHp > 4) {
      screwPositions.emplace_back(right, top);
      screwPositions.emplace_back(left, bottom);
    }

    std::shuffle(screwPositions.begin(), screwPositions.end(), std::mt19937(std::random_device()()));

    auto const positionOfSpecialScrew = screwPositions.back();
    screw<rack::componentlibrary::ScrewBlack>(positionOfSpecialScrew);

    screwPositions.pop_back();

    for (auto const screwPosition : screwPositions) {
      screw<rack::componentlibrary::ScrewSilver>(screwPosition);
    }
  }
};
} // namespace dhe
