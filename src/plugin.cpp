#include "modules/blossom/blossom-panel.h"
#include "modules/blossom/blossom.h"
#include "modules/cubic/Cubic.h"
#include "modules/cubic/CubicPanel.h"
#include "modules/curve-sequencer/CurveSequencerModule.h"
#include "modules/curve-sequencer/CurveSequencerPanel.h"
#include "modules/func/Func1Panel.h"
#include "modules/func/Func6Panel.h"
#include "modules/func/FuncModule.h"
#include "modules/fuzzy-logic/FuzzyLogicH.h"
#include "modules/fuzzy-logic/FuzzyLogicHPanel.h"
#include "modules/fuzzy-logic/FuzzyLogicZ.h"
#include "modules/fuzzy-logic/FuzzyLogicZPanel.h"
#include "modules/gator/Gator.h"
#include "modules/gator/GatorPanel.h"
#include "modules/ranger/Ranger.h"
#include "modules/ranger/RangerPanel.h"
#include "modules/stage/BoosterStageModule.h"
#include "modules/stage/BoosterStagePanel.h"
#include "modules/stage/HostageModule.h"
#include "modules/stage/HostagePanel.h"
#include "modules/stage/StageModule.h"
#include "modules/stage/StagePanel.h"
#include "modules/stage/UpstageModule.h"
#include "modules/stage/UpstagePanel.h"
#include "modules/swave/Swave.h"
#include "modules/swave/SwavePanel.h"
#include "modules/tapers/Tapers.h"
#include "modules/tapers/TapersPanel.h"
#include "modules/xycloid/Xycloid.h"
#include "modules/xycloid/XycloidPanel.h"

#include <plugin/Plugin.hpp>

using dhe::blossom::Blossom;
using dhe::blossom::BlossomPanel;
using dhe::cubic::Cubic;
using dhe::cubic::CubicPanel;
using dhe::curve_sequencer::CurveSequencerModule;
using dhe::curve_sequencer::CurveSequencerPanel;
using dhe::func::Func1Panel;
using dhe::func::Func6Panel;
using dhe::func::FuncModule;
using dhe::fuzzy_logic::FuzzyLogicH;
using dhe::fuzzy_logic::FuzzyLogicHPanel;
using dhe::fuzzy_logic::FuzzyLogicZ;
using dhe::fuzzy_logic::FuzzyLogicZPanel;
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
using dhe::xycloid::Xycloid;
using dhe::xycloid::XycloidPanel;

rack::plugin::Plugin *pluginInstance;

/**
 * Register a model of each module. This funciton is called by Rack when the
 * module is loaded.
 */
extern "C" void init(rack::plugin::Plugin *p) {
  pluginInstance = p;

  p->addModel(rack::createModel<Blossom, BlossomPanel>("Blossom"));
  p->addModel(
      rack::createModel<BoosterStageModule, BoosterStagePanel>("BoosterStage"));
  p->addModel(rack::createModel<Cubic, CubicPanel>("Cubic"));
  p->addModel(
      rack::createModel<CurveSequencerModule<4>, CurveSequencerPanel<4>>(
          "CurveSequencer4"));
  p->addModel(
      rack::createModel<CurveSequencerModule<8>, CurveSequencerPanel<8>>(
          "CurveSequencer8"));
  p->addModel(
      rack::createModel<CurveSequencerModule<16>, CurveSequencerPanel<16>>(
          "CurveSequencer16"));
  p->addModel(rack::createModel<FuncModule<1>, Func1Panel>("Func"));
  p->addModel(rack::createModel<FuncModule<6>, Func6Panel>("Func6"));
  p->addModel(rack::createModel<FuzzyLogicH, FuzzyLogicHPanel>("FuzzyLogicH"));
  p->addModel(rack::createModel<FuzzyLogicZ, FuzzyLogicZPanel>("FuzzyLogicZ"));
  p->addModel(rack::createModel<Gator, GatorPanel>("Gator"));
  p->addModel(rack::createModel<HostageModule, HostagePanel>("Hostage"));
  p->addModel(rack::createModel<Ranger, RangerPanel>("Ranger"));
  p->addModel(rack::createModel<StageModule, StagePanel>("Stage"));
  p->addModel(rack::createModel<Swave, SwavePanel>("Swave"));
  p->addModel(rack::createModel<Tapers, TapersPanel>("Tapers"));
  p->addModel(rack::createModel<UpstageModule, UpstagePanel>("Upstage"));
  p->addModel(rack::createModel<Xycloid, XycloidPanel>("Xycloid"));
}
