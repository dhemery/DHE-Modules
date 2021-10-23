#include "modules/blossom/init.h"
#include "modules/buttons/init.h"
#include "modules/cubic/init.h"
#include "modules/curve-sequencer/init.h"
#include "modules/func/func-module.h"
#include "modules/func/func1-panel.h"
#include "modules/func/func6-panel.h"
#include "modules/fuzzy-logic/init.h"
#include "modules/gator/gator-panel.h"
#include "modules/gator/gator.h"
#include "modules/ranger/ranger-panel.h"
#include "modules/ranger/ranger.h"
#include "modules/scannibal/init.h"
#include "modules/sequencizer/init.h"
#include "modules/stage/booster-stage-module.h"
#include "modules/stage/booster-stage-panel.h"
#include "modules/stage/hostage-module.h"
#include "modules/stage/hostage-panel.h"
#include "modules/stage/stage-module.h"
#include "modules/stage/stage-panel.h"
#include "modules/stage/upstage-module.h"
#include "modules/stage/upstage-panel.h"
#include "modules/swave/swave-panel.h"
#include "modules/swave/swave.h"
#include "modules/tapers/tapers-panel.h"
#include "modules/tapers/tapers.h"
#include "modules/truth/init.h"
#include "modules/xycloid/init.h"

#include "rack.hpp"

using dhe::func::Func1Panel;
using dhe::func::Func6Panel;
using dhe::func::FuncModule;
using dhe::gator::Gator;
using dhe::gator::GatorPanel;
using dhe::ranger::Ranger;
using dhe::ranger::RangerPanel;
using dhe::stage::BoosterStageModule;
using dhe::stage::BoosterStagePanel;
using dhe::stage::HostageModule;
using dhe::stage::HostagePanel;
using dhe::stage::StageModule;
using dhe::stage::StagePanel;
using dhe::stage::UpstageModule;
using dhe::stage::UpstagePanel;
using dhe::swave::Swave;
using dhe::swave::SwavePanel;

using dhe::tapers::Tapers;
using dhe::tapers::TapersPanel;

rack::plugin::Plugin *pluginInstance; // NOLINT

/**
 * Register a model of each module. This funciton is called by Rack when the
 * module is loaded.
 */
extern "C" void init(rack::plugin::Plugin *p) {
  pluginInstance = p;
  dhe::blossom::init(p);
  p->addModel(
      rack::createModel<BoosterStageModule, BoosterStagePanel>("BoosterStage"));
  dhe::buttons::init(p);
  dhe::cubic::init(p);
  dhe::curve_sequencer::init(p);
  p->addModel(rack::createModel<FuncModule<1>, Func1Panel>("Func"));
  p->addModel(rack::createModel<FuncModule<6>, Func6Panel>("Func6")); // NOLINT
  dhe::fuzzy_logic::init(p);
  p->addModel(rack::createModel<Gator, GatorPanel>("Gator"));
  p->addModel(rack::createModel<HostageModule, HostagePanel>("Hostage"));
  p->addModel(rack::createModel<Ranger, RangerPanel>("Ranger"));
  dhe::scannibal::init(p);
  p->addModel(rack::createModel<StageModule, StagePanel>("Stage"));
  p->addModel(rack::createModel<Swave, SwavePanel>("Swave"));
  dhe::sequencizer::init(p);
  p->addModel(rack::createModel<Tapers, TapersPanel>("Tapers"));
  dhe::truth::init(p);
  p->addModel(rack::createModel<UpstageModule, UpstagePanel>("Upstage"));
  dhe::xycloid::init(p);
}
