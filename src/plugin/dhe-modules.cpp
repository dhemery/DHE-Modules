#include <modules/cubic-module.h>
#include "dhe-modules.h"

#include "gui/booster-stage-widget.h"
#include "gui/cubic-widget.h"
#include "gui/hostage-widget.h"
#include "gui/stage-widget.h"
#include "gui/swave-widget.h"
#include "gui/upstage-widget.h"
#include "gui/xycloid-widget.h"
#include "modules/booster-stage-module.h"
#include "modules/cubic-module.h"
#include "modules/hostage-module.h"
#include "modules/stage-module.h"
#include "modules/swave-module.h"
#include "modules/upstage-module.h"
#include "modules/xycloid-module.h"

rack::Plugin *plugin;
rack::Model *modelBoosterStage;
rack::Model *modelCubic;
rack::Model *modelHostage;
rack::Model *modelStage;
rack::Model *modelSwave;
rack::Model *modelUpstage;
rack::Model *modelXycloid;

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
  modelCubic = DHE::createModel<DHE::CubicModule, DHE::CubicWidget, rack::ModelTag>("Cubic", rack::FUNCTION_GENERATOR_TAG);
  modelHostage = DHE::createModel<DHE::HostageModule, DHE::HostageWidget, rack::ModelTag>("Hostage", rack::ENVELOPE_GENERATOR_TAG);
  modelStage = DHE::createModel<DHE::StageModule, DHE::StageWidget, rack::ModelTag>("Stage", rack::ENVELOPE_GENERATOR_TAG);
  modelSwave = DHE::createModel<DHE::SwaveModule, DHE::SwaveWidget, rack::ModelTag>("Swave", rack::WAVESHAPER_TAG);
  modelUpstage = DHE::createModel<DHE::UpstageModule, DHE::UpstageWidget, rack::ModelTag>("Upstage", rack::ENVELOPE_GENERATOR_TAG);
  modelXycloid = DHE::createModel<DHE::XycloidModule, DHE::XycloidWidget, rack::ModelTag>("Xycloid", rack::FUNCTION_GENERATOR_TAG);

  p->addModel(modelBoosterStage);
  p->addModel(modelCubic);
  p->addModel(modelHostage);
  p->addModel(modelStage);
  p->addModel(modelSwave);
  p->addModel(modelUpstage);
  p->addModel(modelXycloid);
}
