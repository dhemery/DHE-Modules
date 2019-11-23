#include "blossom/BlossomPanel.h"
#include "cubic/CubicPanel.h"
#include "curve-sequencer/CurveSequencerModule.h"
#include "curve-sequencer/CurveSequencerPanel.h"
#include "func/Func1Panel.h"
#include "func/Func6Panel.h"
#include "fuzzy-logic/FuzzyLogicHPanel.h"
#include "fuzzy-logic/FuzzyLogicZPanel.h"
#include "gator/GatorPanel.h"
#include "ranger/RangerPanel.h"
#include "stage/BoosterStageModule.h"
#include "stage/BoosterStagePanel.h"
#include "stage/HostageModule.h"
#include "stage/HostagePanel.h"
#include "stage/StageModule.h"
#include "stage/StagePanel.h"
#include "stage/UpstageModule.h"
#include "stage/UpstagePanel.h"
#include "swave/SwavePanel.h"
#include "tapers/TapersPanel.h"
#include "xycloid/XycloidPanel.h"

#include <plugin/Plugin.hpp>
#include <plugin/callbacks.hpp>

rack::plugin::Plugin *pluginInstance;

using dhe::curve_sequencer::CurveSequencerModule;
using dhe::curve_sequencer::CurveSequencerPanel;

void init(rack::plugin::Plugin *p) {
  pluginInstance = p;

  p->addModel(rack::createModel<dhe::Blossom, dhe::BlossomPanel>("Blossom"));
  p->addModel(rack::createModel<dhe::stage::BoosterStageModule, dhe::stage::BoosterStagePanel>("BoosterStage"));
  p->addModel(rack::createModel<CurveSequencerModule<4>, CurveSequencerPanel<4>>("CurveSequencer4"));
  p->addModel(rack::createModel<CurveSequencerModule<8>, CurveSequencerPanel<8>>("CurveSequencer8"));
  p->addModel(rack::createModel<CurveSequencerModule<16>, CurveSequencerPanel<16>>("CurveSequencer16"));
  p->addModel(rack::createModel<dhe::Cubic, dhe::CubicPanel>("Cubic"));
  p->addModel(rack::createModel<dhe::func::FuncModule<1>, dhe::func::Func1Panel>("Func"));
  p->addModel(rack::createModel<dhe::func::FuncModule<6>, dhe::func::Func6Panel>("Func6"));
  p->addModel(rack::createModel<dhe::FuzzyLogicH, dhe::FuzzyLogicHPanel>("FuzzyLogicH"));
  p->addModel(rack::createModel<dhe::FuzzyLogicZ, dhe::FuzzyLogicZPanel>("FuzzyLogicZ"));
  p->addModel(rack::createModel<dhe::Gator, dhe::GatorPanel>("Gator"));
  p->addModel(rack::createModel<dhe::stage::HostageModule, dhe::stage::HostagePanel>("Hostage"));
  p->addModel(rack::createModel<dhe::stage::StageModule, dhe::stage::StagePanel>("Stage"));
  p->addModel(rack::createModel<dhe::Ranger, dhe::RangerPanel>("Ranger"));
  p->addModel(rack::createModel<dhe::Swave, dhe::SwavePanel>("Swave"));
  p->addModel(rack::createModel<dhe::Tapers, dhe::TapersPanel>("Tapers"));
  p->addModel(rack::createModel<dhe::stage::UpstageModule, dhe::stage::UpstagePanel>("Upstage"));
  p->addModel(rack::createModel<dhe::Xycloid, dhe::XycloidPanel>("Xycloid"));
}
