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

  p->addModel(rack::createModel<DHE::StageWidget>(DHE::MANUFACTURER, DHE::Stage::SLUG, DHE::Stage::NAME,
                                                  rack::ENVELOPE_GENERATOR_TAG));
  p->addModel(rack::createModel<DHE::UpstageWidget>(DHE::MANUFACTURER, DHE::Upstage::SLUG, DHE::Upstage::NAME,
                                                  rack::ENVELOPE_GENERATOR_TAG));
}
