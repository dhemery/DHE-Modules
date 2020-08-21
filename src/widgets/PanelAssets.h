#pragma once

#include <app.hpp>
#include <asset.hpp>
#include <string>
#include <window.hpp>

extern rack::plugin::Plugin *pluginInstance;

namespace dhe {
static inline auto load_svg(std::string const &asset_path)
    -> std::shared_ptr<rack::Svg> {
  static auto const plugin_asset_dir =
      rack::asset::plugin(pluginInstance, std::string("svg/"));
  return APP->window->loadSvg(plugin_asset_dir + asset_path);
}

static inline auto background_svg(std::string const &module_svg_dir)
    -> std::shared_ptr<rack::Svg> {
  return load_svg(module_svg_dir + ".svg");
}

static inline auto control_svg(std::string const &module_svg_dir,
                               std::string const &control_name)
    -> std::shared_ptr<rack::Svg> {
  return load_svg(module_svg_dir + "/" + control_name + ".svg");
}
} // namespace dhe
