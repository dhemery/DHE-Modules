#include "dhe-modules.h"

#include <modules/booster-stage-module.h>
#include <modules/booster-stage-widget.h>
#include <modules/stage-module.h>
#include <modules/stage-widget.h>
#include <modules/sport-module.h>
#include <modules/sport-widget.h>
#include "modules/upstage-module.h"
#include "modules/upstage-widget.h"

rack::Plugin *plugin;

namespace DHE {
template<typename TModel, typename TWidget, typename... TTag>
static rack::Model *createModel(std::string moduleSlug, TTag... tags) {
  return rack::Model::create<TModel, TWidget>("DHE-Modules", moduleSlug, moduleSlug, tags...);
}
}

void init(rack::Plugin *p) {
  plugin = p;

  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);
  p->website = "https://github.com/dhemery/DHE-Modules";
  p->manual = "https://github.com/dhemery/DHE-Modules/wiki";

  p->addModel(modelBoosterStage);
  p->addModel(modelSport);
  p->addModel(modelStage);
  p->addModel(modelUpstage);
}

rack::Model
    *modelBoosterStage = DHE::createModel<DHE::BoosterStageModule, DHE::BoosterStageWidget, rack::ModelTag>("Booster Stage", rack::ENVELOPE_GENERATOR_TAG);
rack::Model *modelSport = DHE::createModel<DHE::SportModule, DHE::SportWidget, rack::ModelTag>("Sport", rack::ENVELOPE_GENERATOR_TAG);
rack::Model *modelStage = DHE::createModel<DHE::StageModule, DHE::StageWidget, rack::ModelTag>("Stage", rack::ENVELOPE_GENERATOR_TAG);
rack::Model *modelUpstage = DHE::createModel<DHE::UpstageModule, DHE::UpstageWidget, rack::ModelTag>("Upstage", rack::ENVELOPE_GENERATOR_TAG);
