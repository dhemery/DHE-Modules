#include <modules/cubic-module.h>
#include "dhe-modules.h"

#include "gui/booster-stage-widget.h"
#include "gui/cubic-widget.h"
#include "modules/booster-stage-module.h"
#include "modules/cubic-module.h"

rack::Plugin *plugin;
rack::Model *modelBoosterStage;
rack::Model *modelCubic;

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

  modelBoosterStage = DHE::createModel<DHE::BoosterStageModule, DHE::BoosterStageWidget, rack::ModelTag>("Booster Stage", rack::ENVELOPE_GENERATOR_TAG);
  modelCubic = DHE::createModel<DHE::CubicModule, DHE::CubicWidget, rack::ModelTag>("Cubic", rack::WAVESHAPER_TAG);
  
  p->addModel(modelBoosterStage);
  p->addModel(modelCubic);
  p->addModel(modelHostage);
  p->addModel(modelStage);
  p->addModel(modelSwave);
  p->addModel(modelUpstage);
  p->addModel(modelXycloid);
}
