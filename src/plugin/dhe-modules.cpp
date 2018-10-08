#include <modules/cubic-module.h>
#include "dhe-modules.h"

#include "gui/cubic-widget.h"
#include "modules/cubic-module.h"

rack::Plugin *plugin;

void init(rack::Plugin *p) {
  plugin = p;

  p->slug = "DHE-Modules";
  p->version = TOSTRING(VERSION);

  p->addModel(modelBoosterStage);
  p->addModel(modelCubic);
  p->addModel(modelHostage);
  p->addModel(modelStage);
  p->addModel(modelSwave);
  p->addModel(modelUpstage);
  p->addModel(modelXycloid);
}
