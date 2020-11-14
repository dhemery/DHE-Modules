#pragma once

#include <app.hpp>
#include <asset.hpp>
#include <string>
#include <window.hpp>

extern rack::plugin::Plugin *pluginInstance; // NOLINT

namespace dhe {
static inline auto load_svg(std::string const &module_dir,
                            std::string const &file_name)
    -> std::shared_ptr<rack::Svg> {
  static auto const plugin_svg_dir =
      rack::asset::plugin(pluginInstance, std::string("svg/"));
  return APP->window->loadSvg(plugin_svg_dir + module_dir + "/" + file_name +
                              ".svg");
}
} // namespace dhe
