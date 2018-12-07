#include "dhe-modules.h"

rack::Plugin *plugin;

void init(rack::Plugin *p) {
  plugin = p;

  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);

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
