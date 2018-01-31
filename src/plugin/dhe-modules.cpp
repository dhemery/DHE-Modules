#include <upstage/upstage-widget.h>
#include <upstage/upstage.h>
#include "dhe-modules.h"
#include "stage/stage-widget.h"
#include "stage/stage.h"

rack::Plugin *plugin;

void init(rack::Plugin *p) {
  plugin = p;
  p->slug = DHE::MANUFACTURER;
  p->version = TOSTRING(VERSION);
  p->website = "https://github.com/dhemery/DHE-Modules";
  p->manual = "https://github.com/dhemery/DHE-Modules/wiki";

  DHE::installModule<DHE::StageWidget, rack::ModelTag>(p, "Stage", rack::ENVELOPE_GENERATOR_TAG);
  DHE::installModule<DHE::UpstageWidget, rack::ModelTag>(p, "Upstage", rack::ENVELOPE_GENERATOR_TAG);
}
