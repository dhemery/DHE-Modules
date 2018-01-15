#include "DHEModules.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = "DHE-Modules";
	p->version = TOSTRING(VERSION);
	p->website = "https://github.com/dhemery/DHE-Modules";

	p->addModel(createModel<StageWidget>("DHE-Modules", "Stage", "Stage", ENVELOPE_GENERATOR_TAG));
}
