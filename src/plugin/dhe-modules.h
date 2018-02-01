#ifndef DHE_PLUGIN_DHE_MODULES_H
#define DHE_PLUGIN_DHE_MODULES_H

#include "plugin.hpp"

extern rack::Plugin *plugin;

namespace DHE {
static constexpr char const *MANUFACTURER = "DHE-Modules";

template<typename TWidget, typename... TTag>
static void installModule(rack::Plugin *plugin, std::string moduleSlug, TTag... tags) {
  plugin->addModel(rack::createModel<TWidget>(MANUFACTURER, moduleSlug, moduleSlug, tags...));
}

};
#endif