#include "modules/blossom/init.h"
#include "modules/buttons/init.h"
#include "modules/cubic/init.h"
#include "modules/curve-sequencer/init.h"
#include "modules/func/init.h"
#include "modules/fuzzy-logic/init.h"
#include "modules/gator/init.h"
#include "modules/ranger/init.h"
#include "modules/scannibal/init.h"
#include "modules/sequencizer/init.h"
#include "modules/stage/init.h"
#include "modules/swave/init.h"
#include "modules/tapers/init.h"
#include "modules/truth/init.h"
#include "modules/xycloid/init.h"

#include "rack.hpp"

rack::plugin::Plugin *pluginInstance; // NOLINT

/**
 * Register a model of each module. This function is called by Rack when the
 * module is loaded.
 */
extern "C" void init(rack::plugin::Plugin *p) {
  pluginInstance = p;
  dhe::blossom::init(p);
  dhe::buttons::init(p);
  dhe::cubic::init(p);
  dhe::curve_sequencer::init(p);
  dhe::func::init(p);
  dhe::fuzzy_logic::init(p);
  dhe::gator::init(p);
  dhe::ranger::init(p);
  dhe::scannibal::init(p);
  dhe::sequencizer::init(p);
  dhe::stage::init(p);
  dhe::swave::init(p);
  dhe::tapers::init(p);
  dhe::truth::init(p);
  dhe::xycloid::init(p);
}
