#include "plugin.hpp"

#include "modules/Blossom.h"
#include "panels/BlossomPanel.h"
#include "modules/BoosterStage.h"
#include "panels/BoosterStagePanel.h"
#include "modules/Cubic.h"
#include "panels/CubicPanel.h"
#include "modules/Func.h"
#include "panels/FuncPanel.h"
#include "modules/Func6.h"
#include "panels/Func6Panel.h"
#include "modules/Hostage.h"
#include "panels/HostagePanel.h"
#include "modules/Stage.h"
#include "panels/StagePanel.h"

extern rack::Model *modelRanger;
extern rack::Model *modelSwave;
extern rack::Model *modelTapers;
extern rack::Model *modelUpstage;
extern rack::Model *modelXycloid;

rack::Plugin *plugin;

template<typename TModule, typename TPanel, typename... TTag>
void registerModel(const std::string& name, TTag... tags) {
  rack::Model *model = rack::Model::create<TModule, TPanel, TTag...>("DHE-Modules", name, name, tags...);
  plugin->addModel(model);
}

void init(rack::Plugin *p) {
  plugin = p;

  plugin->slug = "DHE-Modules";
  plugin->version = TOSTRING(VERSION);

  registerModel<DHE::Blossom, DHE::BlossomPanel>("Blossom", rack::LFO_TAG);
  registerModel<DHE::BoosterStage, DHE::BoosterStagePanel>("Booster Stage", rack::ENVELOPE_GENERATOR_TAG);
  registerModel<DHE::Cubic, DHE::CubicPanel>("Cubic", rack::FUNCTION_GENERATOR_TAG);
  registerModel<DHE::Hostage, DHE::HostagePanel>("Hostage", rack::ENVELOPE_GENERATOR_TAG);
  registerModel<DHE::Stage, DHE::StagePanel>("Stage", rack::ENVELOPE_GENERATOR_TAG);
  registerModel<DHE::Func, DHE::FuncPanel>("Func", rack::UTILITY_TAG);
  registerModel<DHE::Func6, DHE::Func6Panel>("Func 6", rack::UTILITY_TAG);

  plugin->addModel(modelRanger);
  plugin->addModel(modelSwave);
  plugin->addModel(modelTapers);
  plugin->addModel(modelUpstage);
  plugin->addModel(modelXycloid);
}
