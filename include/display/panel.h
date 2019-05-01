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

extern rack::plugin::Plugin *plugin;

namespace DHE {
template <typename P> class Jack : public rack::app::SvgPort {
  Jack() { setBackground(P::svg("port")); }
};

template <typename P> class InputJack : public Jack<P> {};

template <typename P> class OutputJack : public Jack<P> {};

static inline auto plugin_asset_dir() -> std::string {
  static const auto dir = rack::asset::plugin(plugin, std::string("svg/"));
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

  void install(float x, float y, InputJack<P> *jack) { addInput(jack); }

  void install(float x, float y, OutputJack<P> *jack) { addOutput(jack); }

  void install(float x, float y, rack::app::ParamWidget *param) {
    addParam(param);
  }

  void install(float x, float y, rack::widget::Widget *widget) {
    addChild(widget);
  }

  template <template <typename> class K>
  auto knob(int index, float initial = 0.5f,
            const std::function<void(float)> &on_change = [](float) {}) const
      -> K<P> * {
    return param<K<P>>(index, 1, initial, on_change);
  }

  template <template <typename> class B = Button>
  auto button(int index, const std::function<void(bool)> &on_change = [](bool) {
  }) const -> B<P> * {
    return param<B<P>>(index, 1, 0, on_change);
  }

  template <template <typename> class C>
  auto toggle(int index, int initial,
              const std::function<void(int)> &on_change = [](int) {}) const
      -> C<P> * {
    return param<C<P>>(index, C<P>::size - 1, initial, on_change);
  }

  template <int N>
  auto toggle(int index, int initial,
              const std::function<void(int)> &on_change = [](int) {}) const
      -> Toggle<P, N> * {
    return param<Toggle<P, N>>(index, N - 1, initial, on_change);
  }

  auto input(int index) const -> InputJack<P> * {
    return rack::createInput({0, 0}, module, index);
  }

  auto output(int index) const -> OutputJack<P> * {
    return rack::createOutput({0, 0}, module, index);
  }

private:
  template <typename T>
  auto param(int index, float max, float initial,
             const std::function<void(float)> &on_change) const -> T * {
    auto widget = rack::createParam<T>({0, 0}, module, index, 0, max, initial);
    widget->notify = on_change;
    widget->notify(widget->value);
    return widget;
  }

  void install_screws() {
    auto screw_diameter =
        rack::app::RACK_GRID_WIDTH * rack::app::MM_PER_IN / rack::app::SVG_DPI;
    auto screw_radius = screw_diameter / 2.f;

    auto top = screw_radius;
    auto bottom = height() - top;

    auto max_screw_inset = screw_diameter * 1.5f;
    auto left = std::min(width() / 4.f, max_screw_inset);
    auto right = width() - left;

    auto screw_positions = std::vector<rack::math::Vec>{
        {left, top}, {left, bottom}, {right, top}, {right, bottom}};

    std::shuffle(screw_positions.begin(), screw_positions.end(),
                 std::mt19937(std::random_device()()));

    auto p_special = screw_positions.back();
    install(p_special.x, p_special.y,
            rack::createWidgetCentered<rack::componentlibrary::ScrewBlack>(p_special));

    screw_positions.pop_back();

    for (auto p : screw_positions) {
      install(rack::createWidgetCentered<rack::componentlibrary::ScrewSilver>(p));
    }
  }
};
} // namespace DHE
