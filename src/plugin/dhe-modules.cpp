#include <modules/cubic-module.h>
#include "dhe-modules.h"

#include "gui/booster-stage-widget.h"
#include "gui/cubic-widget.h"
#include "gui/hostage-widget.h"
#include "gui/stage-widget.h"
#include "modules/booster-stage-module.h"
#include "modules/cubic-module.h"
#include "modules/hostage-module.h"
#include "modules/stage-module.h"

rack::Plugin *plugin;
rack::Model *modelBoosterStage;
rack::Model *modelCubic;
rack::Model *modelHostage;
rack::Model *modelStage;

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
  modelHostage = DHE::createModel<DHE::HostageModule, DHE::HostageWidget, rack::ModelTag>("Hostage", rack::ENVELOPE_GENERATOR_TAG);
  modelStage = DHE::createModel<DHE::StageModule, DHE::StageWidget, rack::ModelTag>("Stage", rack::ENVELOPE_GENERATOR_TAG);

  p->addModel(modelBoosterStage);
  p->addModel(modelCubic);
  p->addModel(modelHostage);
  p->addModel(modelStage);
  p->addModel(modelSwave);
  p->addModel(modelUpstage);
  p->addModel(modelXycloid);
}
