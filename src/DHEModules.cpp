#include "DHEModules.hpp"

using namespace rack;

Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    p->slug = "DHE-Modules";
    p->version = TOSTRING(VERSION);
    p->website = "https://github.com/dhemery/DHE-Modules";

    p->addModel(createModel<DHE::StageWidget>("DHE-Modules", "Stage", "Stage",
                                              ENVELOPE_GENERATOR_TAG));
}
