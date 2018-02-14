#include <modules/stage.h>
#include <modules/upstage.h>
#include "dhe-modules.h"
#include "gui/stage-widget.h"
#include "gui/upstage-widget.h"

rack::Plugin *plugin;

namespace DHE {
template<typename TModel, typename TWidget, typename... TTag>
static rack::Model *createModel(std::string moduleSlug, TTag... tags) {
  rack::debug(">>>createModel(%s)", moduleSlug.c_str());
  return rack::Model::create<TModel, TWidget>("DHE-Modules", moduleSlug, moduleSlug, tags...);
}
}

void init(rack::Plugin *p) {
  rack::debug(">>>init()");
  rack::debug("   modelStage name: %s", modelStage->name.c_str());
  rack::debug("   modelUpstage name: %s", modelUpstage->name.c_str());
  plugin = p;
  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);
  p->website = "https://github.com/dhemery/DHE-Modules";
  p->manual = "https://github.com/dhemery/DHE-Modules/wiki";
  p->addModel(modelStage);
  p->addModel(modelUpstage);
  rack::debug("<<<init()");
}

rack::Model *modelStage = DHE::createModel<DHE::Stage, DHE::StageWidget, rack::ModelTag>("Stage", rack::ENVELOPE_GENERATOR_TAG);
rack::Model *modelUpstage = DHE::createModel<DHE::Upstage, DHE::UpstageWidget, rack::ModelTag>("Upstage", rack::ENVELOPE_GENERATOR_TAG);

