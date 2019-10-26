#include "module/blossom/BlossomPanel.h"
#include "module/cubic/CubicPanel.h"
#include "module/curve-sequencer/CurveSequencerPanel.h"
#include "module/func/Func6Panel.h"
#include "module/func/FuncPanel.h"
#include "module/fuzzy-logic/FuzzyLogicHPanel.h"
#include "module/fuzzy-logic/FuzzyLogicZPanel.h"
#include "module/gator/GatorPanel.h"
#include "module/ranger/RangerPanel.h"
#include "module/stage/BoosterStagePanel.h"
#include "module/stage/HostagePanel.h"
#include "module/stage/StagePanel.h"
#include "module/stage/UpstagePanel.h"
#include "module/swave/SwavePanel.h"
#include "module/tapers/TapersPanel.h"
#include "module/xycloid/XycloidPanel.h"

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
