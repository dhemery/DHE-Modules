#include "gui/stage-widget.h"
#include "gui/upstage-widget.h"

rack::Plugin *plugin;

namespace DHE {
template<typename TWidget, typename... TTag>
void installModule(std::string moduleSlug, TTag... tags) {
  plugin->addModel(rack::createModel<TWidget>(plugin->slug, moduleSlug, moduleSlug, tags...));
}
}

void init(rack::Plugin *p) {
  plugin = p;
  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);
  p->website = "https://github.com/dhemery/DHE-Modules";
  p->manual = "https://github.com/dhemery/DHE-Modules/wiki";

  DHE::installModule<DHE::StageWidget, rack::ModelTag>("Stage", rack::ENVELOPE_GENERATOR_TAG);
  DHE::installModule<DHE::UpstageWidget, rack::ModelTag>("Upstage", rack::ENVELOPE_GENERATOR_TAG);
}

