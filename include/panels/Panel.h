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
template <typename P> class Jack : public rack::app::SvgPort {
public:
  Jack() { setSvg(P::svg("port")); }
};

template <typename P> class InputJack : public Jack<P> {};

template <typename P> class OutputJack : public Jack<P> {};

static inline auto mmvec(float x, float y) -> rack::math::Vec { return rack::app::mm2px(rack::math::Vec{x, y}); }

static inline auto plugin_asset_dir() -> std::string {
  static const auto dir = rack::asset::plugin(pluginInstance, std::string("svg/"));
  return dir;
}

template <typename P> class Panel : public rack::app::ModuleWidget {
public:
  Panel(rack::engine::Module *module, int widget_hp) {
    setModule(module);
    box.size = rack::math::Vec{widget_hp * rack::app::RACK_GRID_WIDTH, rack::app::RACK_GRID_HEIGHT};

    auto panel = new rack::app::SvgPanel();
    panel->setBackground(panel_svg());
    addChild(panel);

    install_screws();
  }

  static auto svg(const std::string &filename) -> std::shared_ptr<rack::Svg> {
    static const auto module_asset_dir = plugin_asset_dir() + P::module_slug + "/";
    return rack::APP->window->loadSvg(module_asset_dir + filename + ".svg");
  }

  static auto panel_svg() -> std::shared_ptr<rack::Svg> {
    return rack::APP->window->loadSvg(plugin_asset_dir() + P::module_slug + ".svg");
  }

protected:
  auto height() const -> float { return box.size.y * rack::app::MM_PER_IN / rack::app::SVG_DPI; }

  auto width() const -> float { return box.size.x * rack::app::MM_PER_IN / rack::app::SVG_DPI; }

  template <typename T> auto param(float x, float y, int index) -> T * {
    auto const &pos = mmvec(x, y);
    auto *widget = rack::createParamCentered<T>(pos, module, index);
    widget->shadow->opacity = 0.f;
    addParam(widget);
    return widget;
  }

  template <template <typename> class K> auto knob(float x, float y, int index) -> K<P> * {
    return param<K<P>>(x, y, index);
  }

  template <template <typename> class B = Button> auto button(float x, float y, int index) -> B<P> * {
    return param<B<P>>(x, y, index);
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
    input->shadow->opacity = 0.f;
    addInput(input);
  }

  void output(float x, float y, int index) {
    auto const &pos = mmvec(x, y);
    auto *output = rack::createOutputCentered<OutputJack<P>>(pos, module, index);
    output->shadow->opacity = 0.f;
    addOutput(output);
  }

private:
  template <typename T> void screw(rack::math::Vec pos) {
    addChild(rack::createWidgetCentered<T>(rack::app::mm2px(pos)));
  }

  void install_screws() {
    auto screw_diameter = rack::app::RACK_GRID_WIDTH * rack::app::MM_PER_IN / rack::app::SVG_DPI;
    auto screw_radius = screw_diameter / 2.f;

    auto top = screw_radius;
    auto bottom = height() - top;

    auto max_screw_inset = screw_diameter * 1.5f;
    auto left = std::min(width() / 4.f, max_screw_inset);
    auto right = width() - left;

    auto screw_positions = std::vector<rack::math::Vec>{{left, top}, {left, bottom}, {right, top}, {right, bottom}};

    std::shuffle(screw_positions.begin(), screw_positions.end(), std::mt19937(std::random_device()()));

    auto p_special = screw_positions.back();
    screw<rack::componentlibrary::ScrewBlack>(p_special);

    screw_positions.pop_back();

    for (auto p : screw_positions) {
      screw<rack::componentlibrary::ScrewSilver>(p);
    }
  }
};
} // namespace dhe
