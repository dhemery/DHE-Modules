#include "plugin.hpp"

#include "modules/Cubic.h"
#include "panels/CubicPanel.h"

extern rack::Model *modelBlossom;
extern rack::Model *modelBoosterStage;
extern rack::Model *modelFunc;
extern rack::Model *modelFunc6;
extern rack::Model *modelHostage;
extern rack::Model *modelRanger;
extern rack::Model *modelStage;
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

  plugin->addModel(modelBlossom);
  plugin->addModel(modelBoosterStage);

  registerModel<DHE::Cubic, DHE::CubicPanel>("Cubic", rack::FUNCTION_GENERATOR_TAG);

  plugin->addModel(modelFunc);
  plugin->addModel(modelFunc6);
  plugin->addModel(modelHostage);
  plugin->addModel(modelRanger);
  plugin->addModel(modelStage);
  plugin->addModel(modelSwave);
  plugin->addModel(modelTapers);
  plugin->addModel(modelUpstage);
  plugin->addModel(modelXycloid);
}
