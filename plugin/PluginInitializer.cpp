#include "modules/blossom/BlossomPanel.h"
#include "modules/cubic/CubicPanel.h"
#include "modules/curve-sequencer/CurveSequencerPanel.h"
#include "modules/func/Func6Panel.h"
#include "modules/func/FuncPanel.h"
#include "modules/fuzzy-logic/FuzzyLogicHPanel.h"
#include "modules/fuzzy-logic/FuzzyLogicZPanel.h"
#include "modules/gator/GatorPanel.h"
#include "modules/ranger/RangerPanel.h"
#include "modules/stage/BoosterStagePanel.h"
#include "modules/stage/HostagePanel.h"
#include "modules/stage/StagePanel.h"
#include "modules/stage/UpstagePanel.h"
#include "modules/swave/SwavePanel.h"
#include "modules/tapers/TapersPanel.h"
#include "modules/xycloid/XycloidPanel.h"

#include <plugin/Plugin.hpp>
#include <plugin/callbacks.hpp>

rack::plugin::Plugin *pluginInstance;

void init(rack::plugin::Plugin *p) {
  pluginInstance = p;

  p->addModel(rack::createModel<dhe::Blossom, dhe::BlossomPanel>("Blossom"));
  p->addModel(rack::createModel<dhe::BoosterStage, dhe::BoosterStagePanel>("BoosterStage"));
  p->addModel(rack::createModel<dhe::CurveSequencer<4>, dhe::CurveSequencerPanel<4>>("CurveSequencer4"));
  p->addModel(rack::createModel<dhe::CurveSequencer<8>, dhe::CurveSequencerPanel<8>>("CurveSequencer8"));
  p->addModel(rack::createModel<dhe::CurveSequencer<16>, dhe::CurveSequencerPanel<16>>("CurveSequencer16"));
  p->addModel(rack::createModel<dhe::Cubic, dhe::CubicPanel>("Cubic"));
  p->addModel(rack::createModel<dhe::Func, dhe::FuncPanel>("Func"));
  p->addModel(rack::createModel<dhe::FuzzyLogicH, dhe::FuzzyLogicHPanel>("FuzzyLogicH"));
  p->addModel(rack::createModel<dhe::FuzzyLogicZ, dhe::FuzzyLogicZPanel>("FuzzyLogicZ"));
  p->addModel(rack::createModel<dhe::Func6, dhe::Func6Panel>("Func6"));
  p->addModel(rack::createModel<dhe::Gator, dhe::GatorPanel>("Gator"));
  p->addModel(rack::createModel<dhe::Hostage, dhe::HostagePanel>("Hostage"));
  p->addModel(rack::createModel<dhe::Stage, dhe::StagePanel>("Stage"));
  p->addModel(rack::createModel<dhe::Ranger, dhe::RangerPanel>("Ranger"));
  p->addModel(rack::createModel<dhe::Swave, dhe::SwavePanel>("Swave"));
  p->addModel(rack::createModel<dhe::Tapers, dhe::TapersPanel>("Tapers"));
  p->addModel(rack::createModel<dhe::Upstage, dhe::UpstagePanel>("Upstage"));
  p->addModel(rack::createModel<dhe::Xycloid, dhe::XycloidPanel>("Xycloid"));
}
