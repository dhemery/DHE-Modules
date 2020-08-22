#include "modules/blossom/blossom-panel.h"
#include "modules/blossom/blossom.h"
#include "modules/cubic/cubic-panel.h"
#include "modules/cubic/cubic.h"
#include "modules/curve-sequencer/curve-sequencer-module.h"
#include "modules/curve-sequencer/curve-sequencer-panel.h"
#include "modules/func/func-module.h"
#include "modules/func/func1-panel.h"
#include "modules/func/func6-panel.h"
#include "modules/fuzzy-logic/h-module.h"
#include "modules/fuzzy-logic/h-panel.h"
#include "modules/fuzzy-logic/z-module.h"
#include "modules/fuzzy-logic/z-panel.h"
#include "modules/gator/gator-panel.h"
#include "modules/gator/gator.h"
#include "modules/ranger/ranger-panel.h"
#include "modules/ranger/ranger.h"
#include "modules/stage/BoosterStageModule.h"
#include "modules/stage/BoosterStagePanel.h"
#include "modules/stage/HostageModule.h"
#include "modules/stage/HostagePanel.h"
#include "modules/stage/StageModule.h"
#include "modules/stage/StagePanel.h"
#include "modules/stage/UpstageModule.h"
#include "modules/stage/UpstagePanel.h"
#include "modules/swave/swave-panel.h"
#include "modules/swave/swave.h"
#include "modules/tapers/tapers-panel.h"
#include "modules/tapers/tapers.h"
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
using dhe::fuzzy_logic::HModule;
using dhe::fuzzy_logic::HPanel;
using dhe::fuzzy_logic::ZModule;
using dhe::fuzzy_logic::ZPanel;
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
  p->addModel(rack::createModel<HModule, HPanel>("FuzzyLogicH"));
  p->addModel(rack::createModel<ZModule, ZPanel>("FuzzyLogicZ"));
  p->addModel(rack::createModel<Gator, GatorPanel>("Gator"));
  p->addModel(rack::createModel<HostageModule, HostagePanel>("Hostage"));
  p->addModel(rack::createModel<Ranger, RangerPanel>("Ranger"));
  p->addModel(rack::createModel<StageModule, StagePanel>("Stage"));
  p->addModel(rack::createModel<Swave, SwavePanel>("Swave"));
  p->addModel(rack::createModel<Tapers, TapersPanel>("Tapers"));
  p->addModel(rack::createModel<UpstageModule, UpstagePanel>("Upstage"));
  p->addModel(rack::createModel<Xycloid, XycloidPanel>("Xycloid"));
}
