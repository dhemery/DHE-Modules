#include "dhe-modules.hpp"

rack::Plugin *plugin;

void init(rack::Plugin *p) {
  plugin = p;

  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);

  p->addModel(modelBoosterStage);
  p->addModel(modelCubic);
  p->addModel(modelHostage);
  p->addModel(modelJuster);
  p->addModel(modelRanger);
  p->addModel(modelStage);
  p->addModel(modelSwave);
  p->addModel(modelTapers);
  p->addModel(modelUpstage);
  p->addModel(modelXycloid);
}
