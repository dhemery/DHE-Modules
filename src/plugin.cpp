#include "modules/blossom/blossom-panel.h"
#include "modules/blossom/blossom.h"
#include "modules/cubic/cubic-panel.h"
#include "modules/cubic/cubic.h"
#include "modules/curve-sequencer/curve-sequencer-module.h"
#include "modules/curve-sequencer/curve-sequencer-panel.h"
#include "modules/func/func-module.h"
#include "modules/func/func1-panel.h"
#include "modules/func/func6-panel.h"
#include "modules/fuzzy-logic/fuzzy-logic-h-module.h"
#include "modules/fuzzy-logic/fuzzy-logic-h-panel.h"
#include "modules/fuzzy-logic/fuzzy-logic-z-module.h"
#include "modules/fuzzy-logic/fuzzy-logic-z-panel.h"
#include "modules/gator/gator-panel.h"
#include "modules/gator/gator.h"
#include "modules/ranger/ranger-panel.h"
#include "modules/ranger/ranger.h"
#include "modules/scannable/module.h"
#include "modules/scannable/panel.h"
#include "modules/sequencizer/module.h"
#include "modules/sequencizer/panel.h"
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
#include "modules/xycloid/xycloid-panel.h"
#include "modules/xycloid/xycloid.h"

#include <plugin/Plugin.hpp>

using dhe::blossom::Blossom;
using dhe::blossom::BlossomPanel;
using dhe::cubic::Cubic;
using dhe::cubic::CubicPanel;
using dhe::func::Func1Panel;
using dhe::func::Func6Panel;
using dhe::func::FuncModule;
using dhe::fuzzy_logic::FuzzyLogicHModule;
using dhe::fuzzy_logic::FuzzyLogicHPanel;
using dhe::fuzzy_logic::FuzzyLogicZModule;
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

template <int N> using SequencizerModule = dhe::sequencizer::Module<N>;
template <int N> using SequencizerPanel = dhe::sequencizer::Panel<N>;
template <int N> using ScannableModule = dhe::scannable::Module<N>;
template <int N> using ScannablePanel = dhe::scannable::Panel<N>;
using dhe::tapers::Tapers;
using dhe::tapers::TapersPanel;
using dhe::xycloid::Xycloid;
using dhe::xycloid::XycloidPanel;

rack::plugin::Plugin *pluginInstance; // NOLINT

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
  p->addModel(rack::createModel<dhe::curve_sequencer::CurveSequencerModule<4>,
                                dhe::curve_sequencer::CurveSequencerPanel<4>>(
      "CurveSequencer4"));
  p->addModel(rack::createModel<dhe::curve_sequencer::CurveSequencerModule<8>,
                                dhe::curve_sequencer::CurveSequencerPanel<8>>(
      "CurveSequencer8"));
  p->addModel(rack::createModel<dhe::curve_sequencer::CurveSequencerModule<16>,
                                dhe::curve_sequencer::CurveSequencerPanel<16>>(
      "CurveSequencer16"));
  p->addModel(rack::createModel<FuncModule<1>, Func1Panel>("Func"));
  p->addModel(rack::createModel<FuncModule<6>, Func6Panel>("Func6")); // NOLINT
  p->addModel(
      rack::createModel<FuzzyLogicHModule, FuzzyLogicHPanel>("FuzzyLogicH"));
  p->addModel(
      rack::createModel<FuzzyLogicZModule, FuzzyLogicZPanel>("FuzzyLogicZ"));
  p->addModel(rack::createModel<Gator, GatorPanel>("Gator"));
  p->addModel(rack::createModel<HostageModule, HostagePanel>("Hostage"));
  p->addModel(rack::createModel<Ranger, RangerPanel>("Ranger"));
  p->addModel(
      rack::createModel<ScannableModule<4>, ScannablePanel<4>>("Scannable4"));
  p->addModel(
      rack::createModel<ScannableModule<8>, ScannablePanel<8>>("Scannable8"));
  p->addModel(rack::createModel<ScannableModule<16>, ScannablePanel<16>>(
      "Scannable16"));
  p->addModel(rack::createModel<StageModule, StagePanel>("Stage"));
  p->addModel(rack::createModel<Swave, SwavePanel>("Swave"));
  p->addModel(rack::createModel<SequencizerModule<4>, SequencizerPanel<4>>(
      "Sequencizer4"));
  p->addModel(rack::createModel<SequencizerModule<8>, SequencizerPanel<8>>(
      "Sequencizer8"));
  p->addModel(rack::createModel<SequencizerModule<16>, SequencizerPanel<16>>(
      "Sequencizer16"));
  p->addModel(rack::createModel<Tapers, TapersPanel>("Tapers"));
  p->addModel(rack::createModel<UpstageModule, UpstagePanel>("Upstage"));
  p->addModel(rack::createModel<Xycloid, XycloidPanel>("Xycloid"));
}
