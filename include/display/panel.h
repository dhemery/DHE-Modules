#pragma once
#include <algorithm>
#include <random>
#include <string>
#include <utility>

#include "app.hpp"
#include "event.hpp"
#include "helpers.hpp"
#include "math.hpp"

#include "app/ModuleWidget.hpp"
#include "app/ParamWidget.hpp"
#include "app/PortWidget.hpp"
#include "app/SvgPanel.hpp"
#include "app/SvgPort.hpp"
#include "app/common.hpp"
#include "widget/Widget.hpp"

#include "display/controls.h"

extern rack::plugin::Plugin *pluginInstance;

namespace DHE {
template <typename P> class Jack : public rack::app::SvgPort {
public:
  Jack() { setSvg(P::svg("port")); }
};

template <typename P> class InputJack : public Jack<P> {};

template <typename P> class OutputJack : public Jack<P> {};

static inline auto mmvec(float x, float y) -> rack::math::Vec {
  return rack::app::mm2px(rack::math::Vec{x, y});
}

static inline auto plugin_asset_dir() -> std::string {
  static const auto dir =
      rack::asset::plugin(pluginInstance, std::string("svg/"));
  return dir;
}

template <typename P> class Panel : public rack::app::ModuleWidget {
public:
  Panel(rack::engine::Module *module, int widget_hp) {
    setModule(module);
    box.size = rack::math::Vec{widget_hp * rack::app::RACK_GRID_WIDTH,
                               rack::app::RACK_GRID_HEIGHT};

    auto panel = new rack::app::SvgPanel();
    panel->setBackground(panel_svg());
    addChild(panel);

    install_screws();
  }

  void fromJson(json_t *patch) override {
    // If there's no data, we're loading from a legacy patch. Add empty data to
    // the incoming patch so that ModuleWidget::fromJson will call
    // Module::fromJson, which will configure the module with appropriate legacy
    // behavior.
    if (!json_object_get(patch, "data")) {
      json_object_set_new(patch, "data", json_object());
    }
    rack::app::ModuleWidget::fromJson(patch);
  }

  static auto svg(const std::string &filename) -> std::shared_ptr<rack::Svg> {
    static const auto module_asset_dir =
        plugin_asset_dir() + P::module_slug + "/";
    return rack::APP->window->loadSvg(module_asset_dir + filename + ".svg");
  }

  static auto panel_svg() -> std::shared_ptr<rack::Svg> {
    return rack::APP->window->loadSvg(plugin_asset_dir() + P::module_slug +
                                      ".svg");
  }

protected:
  auto height() const -> float {
    return box.size.y * rack::app::MM_PER_IN / rack::app::SVG_DPI;
  }

  auto width() const -> float {
    return box.size.x * rack::app::MM_PER_IN / rack::app::SVG_DPI;
  }

  template <template <typename> class K>
  void knob(float x, float y, int index) {
    addParam(rack::createParamCentered<K<P>>(mmvec(x, y), module, index));
  }

  template <template <typename> class B = Button>
  void button(float x, float y, int index) {
    addParam(rack::createParamCentered<B<P>>(mmvec(x, y), module, index));
  }

  template <template <typename> class C>
  void toggle(float x, float y, int index) {
    addParam(rack::createParamCentered<C<P>>(mmvec(x, y), module, index));
  }

  template <int N> void toggle(float x, float y, int index) {
    addParam(
        rack::createParamCentered<Toggle<P, N>>(mmvec(x, y), module, index));
  }

  void input(float x, float y, int index) {
    addInput(
        rack::createInputCentered<InputJack<P>>(mmvec(x, y), module, index));
  }

  void output(float x, float y, int index) {
    addOutput(
        rack::createOutputCentered<OutputJack<P>>(mmvec(x, y), module, index));
  }

private:
  void install_screws() {
    auto screw_diameter =
        rack::app::RACK_GRID_WIDTH * rack::app::MM_PER_IN / rack::app::SVG_DPI;
    auto screw_radius = screw_diameter / 2.f;

    auto top = screw_radius;
    auto bottom = height() - top;

    auto max_screw_inset = screw_diameter * 1.5f;
    auto left = std::min(width() / 4.f, max_screw_inset);
    auto right = width() - left;

    auto screw_positions =
        std::vector<rack::math::Vec>{mmvec(left, top), mmvec(left, bottom),
                                     mmvec(right, top), mmvec(right, bottom)};

    std::shuffle(screw_positions.begin(), screw_positions.end(),
                 std::mt19937(std::random_device()()));

    auto p_special = screw_positions.back();
    addChild(rack::createWidgetCentered<rack::componentlibrary::ScrewBlack>(
        p_special));

    screw_positions.pop_back();

    for (auto p : screw_positions) {
      addChild(rack::createWidgetCentered<rack::componentlibrary::ScrewSilver>(p));
    }
  }
};
} // namespace DHE
