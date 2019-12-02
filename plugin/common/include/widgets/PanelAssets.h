#pragma once

#include <app.hpp>
#include <asset.hpp>
#include <string>
#include <window.hpp>

extern rack::plugin::Plugin *pluginInstance;

namespace dhe {
static inline auto loadSvg(std::string const &assetPath) -> std::shared_ptr<rack::Svg> {
  static auto const pluginAssetDir = rack::asset::plugin(pluginInstance, std::string("svg/"));
  return APP->window->loadSvg(pluginAssetDir + assetPath);
}

static inline auto backgroundSvg(std::string const &moduleSvgDir) -> std::shared_ptr<rack::Svg> {
  return loadSvg(moduleSvgDir + ".svg");
}

static inline auto controlSvg(std::string const &moduleSvgDir, std::string const &controlName)
    -> std::shared_ptr<rack::Svg> {
  return loadSvg(moduleSvgDir + "/" + controlName + ".svg");
}
} // namespace dhe
