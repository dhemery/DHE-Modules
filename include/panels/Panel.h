#pragma once
#include "panels/widgets/Buttons.h"
#include "panels/widgets/Knobs.h"
#include "panels/widgets/Toggles.h"

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

extern rack::plugin::Plugin *pluginInstance;

namespace dhe {
static inline auto mmPerHp() -> float {
  static auto const mmPerHp = rack::app::RACK_GRID_WIDTH * rack::app::MM_PER_IN / rack::app::SVG_DPI;
  return mmPerHp;
}
static inline auto hp2px(float hp) -> float { return rack::app::RACK_GRID_WIDTH * (float) hp; }
static inline auto hp2mm(float hp) -> float { return hp * mmPerHp(); }

template <typename P> class Jack : public rack::app::SvgPort {
public:
  Jack() { setSvg(P::svg("port")); }
};

template <typename P> class InputJack : public Jack<P> {};

template <typename P> class OutputJack : public Jack<P> {};

static inline auto mmvec(float x, float y) -> rack::math::Vec { return rack::app::mm2px(rack::math::Vec{x, y}); }

static inline auto pluginAssetDir() -> std::string {
  static const auto dir = rack::asset::plugin(pluginInstance, std::string("svg/"));
  return dir;
}

template <typename P> class Panel : public rack::app::ModuleWidget {
public:
  Panel(rack::engine::Module *module, int widgetHp) {
    setModule(module);
    box.size = rack::math::Vec{hp2px(widgetHp), rack::app::RACK_GRID_HEIGHT};

    auto panel = new rack::app::SvgPanel();
    panel->setBackground(panelSvg());
    addChild(panel);

    installScrews(widgetHp);
  }

  static auto svg(const std::string &filename) -> std::shared_ptr<rack::Svg> {
    static const auto moduleAssetDir = pluginAssetDir() + P::moduleSlug + "/";
    return rack::APP->window->loadSvg(moduleAssetDir + filename + ".svg");
  }

  static auto panelSvg() -> std::shared_ptr<rack::Svg> {
    return rack::APP->window->loadSvg(pluginAssetDir() + P::moduleSlug + ".svg");
  }

protected:
  auto height() const -> float { return box.size.y * rack::app::MM_PER_IN / rack::app::SVG_DPI; }

  auto width() const -> float { return box.size.x * rack::app::MM_PER_IN / rack::app::SVG_DPI; }

  template <typename T> auto param(float x, float y, int index) -> T * {
    auto const &pos = mmvec(x, y);
    auto *widget = rack::createParamCentered<T>(pos, module, index);
    widget->shadow->opacity = 0.F;
    addParam(widget);
    return widget;
  }

  template <template <typename> class K> auto knob(float x, float y, int index) -> K<P> * {
    return param<K<P>>(x, y, index);
  }

  template <template <typename> class B = Button> auto button(float x, float y, int index) -> B<P> * {
    return param<B<P>>(x, y, index);
  }

  void light(float x, float y, int index) {
    auto const &pos = mmvec(x, y);
    auto *light = rack::createLightCentered<rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>>(
        pos, module, index);
    addChild(light);
  }

  template <template <typename> class C> auto toggle(float x, float y, int index) -> C<P> * {
    return param<C<P>>(x, y, index);
  }

  template <template <typename, int> class C, int N> auto toggle(float x, float y, int index) -> C<P, N> * {
    return param<C<P, N>>(x, y, index);
  }

  template <int N> auto toggle(float x, float y, int index) -> Toggle<P, N> * {
    return param<Toggle<P, N>>(x, y, index);
  }

  void input(float x, float y, int index) {
    auto const &pos = mmvec(x, y);
    auto *input = rack::createInputCentered<InputJack<P>>(pos, module, index);
    input->shadow->opacity = 0.F;
    addInput(input);
  }

  void output(float x, float y, int index) {
    auto const &pos = mmvec(x, y);
    auto *output = rack::createOutputCentered<OutputJack<P>>(pos, module, index);
    output->shadow->opacity = 0.F;
    addOutput(output);
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
