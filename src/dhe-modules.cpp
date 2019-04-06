//namespace rack {
//struct Plugin;
//struct Model;
//} // namespace rack

#include "plugin.hpp"
#include "util/common.hpp"

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

rack::Model *modelCubic = rack::Model::create<DHE::Cubic, DHE::CubicPanel>(
    "DHE-Modules", "Cubic", "Cubic", rack::FUNCTION_GENERATOR_TAG);

void init(rack::Plugin *p) {
  plugin = p;

  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);

  p->addModel(modelBlossom);
  p->addModel(modelBoosterStage);
  p->addModel(modelCubic);
  p->addModel(modelFunc);
  p->addModel(modelFunc6);
  p->addModel(modelHostage);
  p->addModel(modelRanger);
  p->addModel(modelStage);
  p->addModel(modelSwave);
  p->addModel(modelTapers);
  p->addModel(modelUpstage);
  p->addModel(modelXycloid);
}

rack::Plugin *plugin;
