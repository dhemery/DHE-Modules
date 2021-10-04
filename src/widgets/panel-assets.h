#pragma once

#include <string>

#include <asset.hpp>
#include <context.hpp>
#include <window/Svg.hpp>
#include <window/Window.hpp>

extern rack::plugin::Plugin *pluginInstance; // NOLINT

namespace dhe {
static inline auto load_svg(std::string const &file_name)
    -> std::shared_ptr<rack::window::Svg> {
  static auto const plugin_svg_dir =
      rack::asset::plugin(pluginInstance, std::string("svg/"));
  return APP->window->loadSvg(plugin_svg_dir + file_name + ".svg");
}

static inline auto load_svg(std::string const &module_dir,
                            std::string const &file_name)
    -> std::shared_ptr<rack::window::Svg> {
  return load_svg(module_dir + "/" + file_name);
}
} // namespace dhe
