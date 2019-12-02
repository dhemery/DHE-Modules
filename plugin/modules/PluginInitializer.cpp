#include "blossom/Blossom.h"
#include "blossom/BlossomPanel.h"
#include "cubic/Cubic.h"
#include "cubic/CubicPanel.h"
#include "curve-sequencer/CurveSequencerModule.h"
#include "curve-sequencer/CurveSequencerPanel.h"
#include "func/Func1Panel.h"
#include "func/Func6Panel.h"
#include "func/FuncModule.h"
#include "fuzzy-logic/FuzzyLogicH.h"
#include "fuzzy-logic/FuzzyLogicHPanel.h"
#include "fuzzy-logic/FuzzyLogicZ.h"
#include "fuzzy-logic/FuzzyLogicZPanel.h"
#include "gator/Gator.h"
#include "gator/GatorPanel.h"
#include "ranger/Ranger.h"
#include "ranger/RangerPanel.h"
#include "stage/BoosterStageModule.h"
#include "stage/BoosterStagePanel.h"
#include "stage/HostageModule.h"
#include "stage/HostagePanel.h"
#include "stage/StageModule.h"
#include "stage/StagePanel.h"
#include "stage/UpstageModule.h"
#include "stage/UpstagePanel.h"
#include "swave/Swave.h"
#include "swave/SwavePanel.h"
#include "tapers/Tapers.h"
#include "tapers/TapersPanel.h"
#include "xycloid/Xycloid.h"
#include "xycloid/XycloidPanel.h"

#include <plugin/Plugin.hpp>
#include <plugin/callbacks.hpp>

rack::plugin::Plugin *pluginInstance;

using dhe::blossom::Blossom;
using dhe::blossom::BlossomPanel;
using dhe::curve_sequencer::CurveSequencerModule;
using dhe::curve_sequencer::CurveSequencerPanel;
using dhe::func::Func1Panel;
using dhe::func::Func6Panel;
using dhe::func::FuncModule;
using dhe::stage::BoosterStageModule;
using dhe::stage::BoosterStagePanel;
using dhe::stage::HostageModule;
using dhe::stage::HostagePanel;
using dhe::stage::StageModule;
using dhe::stage::StagePanel;
using dhe::stage::UpstageModule;
using dhe::stage::UpstagePanel;
using dhe::xycloid::Xycloid;
using dhe::xycloid::XycloidPanel;

void init(rack::plugin::Plugin *p) {
  pluginInstance = p;

  p->addModel(rack::createModel<Blossom, BlossomPanel>("Blossom"));
  p->addModel(rack::createModel<BoosterStageModule, BoosterStagePanel>("BoosterStage"));
  p->addModel(rack::createModel<CurveSequencerModule<4>, CurveSequencerPanel<4>>("CurveSequencer4"));
  p->addModel(rack::createModel<CurveSequencerModule<8>, CurveSequencerPanel<8>>("CurveSequencer8"));
  p->addModel(rack::createModel<CurveSequencerModule<16>, CurveSequencerPanel<16>>("CurveSequencer16"));
  p->addModel(rack::createModel<dhe::Cubic, dhe::CubicPanel>("Cubic"));
  p->addModel(rack::createModel<FuncModule<1>, Func1Panel>("Func"));
  p->addModel(rack::createModel<FuncModule<6>, Func6Panel>("Func6"));
  p->addModel(rack::createModel<dhe::FuzzyLogicH, dhe::FuzzyLogicHPanel>("FuzzyLogicH"));
  p->addModel(rack::createModel<dhe::FuzzyLogicZ, dhe::FuzzyLogicZPanel>("FuzzyLogicZ"));
  p->addModel(rack::createModel<dhe::Gator, dhe::GatorPanel>("Gator"));
  p->addModel(rack::createModel<HostageModule, HostagePanel>("Hostage"));
  p->addModel(rack::createModel<StageModule, StagePanel>("Stage"));
  p->addModel(rack::createModel<dhe::Ranger, dhe::RangerPanel>("Ranger"));
  p->addModel(rack::createModel<dhe::Swave, dhe::SwavePanel>("Swave"));
  p->addModel(rack::createModel<dhe::Tapers, dhe::TapersPanel>("Tapers"));
  p->addModel(rack::createModel<UpstageModule, UpstagePanel>("Upstage"));
  p->addModel(rack::createModel<Xycloid, XycloidPanel>("Xycloid"));
}
