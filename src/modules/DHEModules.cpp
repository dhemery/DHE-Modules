#include "rack.hpp"
#include "DHEModules.hpp"
#include "StageWidget.hpp"
#include "Stage.hpp"

rack::Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    p->slug = DHE::MANUFACTURER;
    p->version = TOSTRING(VERSION);
    p->website = "https://github.com/dhemery/DHE-Modules";
    p->manual = "https://github.com/dhemery/DHE-Modules/wiki";

    p->addModel(rack::createModel<DHE::StageWidget>(DHE::MANUFACTURER, DHE::Stage::SLUG, DHE::Stage::NAME,
                                                    rack::ENVELOPE_GENERATOR_TAG));
}
/**/