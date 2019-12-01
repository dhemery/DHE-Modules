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

template <typename P> auto backgroundSvg() -> std::shared_ptr<rack::Svg> {
  return loadSvg(std::string{P::moduleSlug} + ".svg");
}

template <typename P> static inline auto controlSvg(std::string const &controlName) -> std::shared_ptr<rack::Svg> {
  static auto const moduleAssetDir = std::string{P::moduleSlug} + "/";
  return loadSvg(moduleAssetDir + controlName + ".svg");
}
} // namespace dhe
