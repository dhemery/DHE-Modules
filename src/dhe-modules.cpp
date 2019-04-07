#include <algorithm>

#include "plugin.hpp"

#include "modules/Blossom.h"
#include "panels/BlossomPanel.h"
#include "modules/BoosterStage.h"
#include "panels/BoosterStagePanel.h"
#include "modules/Cubic.h"
#include "panels/CubicPanel.h"
#include "modules/Func.h"
#include "panels/FuncPanel.h"
#include "modules/Func6.h"
#include "panels/Func6Panel.h"
#include "modules/Hostage.h"
#include "panels/HostagePanel.h"
#include "modules/Ranger.h"
#include "panels/RangerPanel.h"
#include "modules/Stage.h"
#include "panels/StagePanel.h"
#include "modules/Swave.h"
#include "panels/SwavePanel.h"
#include "modules/Tapers.h"
#include "panels/TapersPanel.h"
#include "modules/Upstage.h"
#include "panels/UpstagePanel.h"

extern rack::Model *modelXycloid;

rack::Plugin *plugin;

template<typename TModule, typename TPanel, typename... TTag>
void registerModel(const std::string& name, TTag... tags) {
  auto slug = name;
  // Slug = name with spaces removed
  slug.erase(std::remove(slug.begin(), slug.end(), ' '), slug.end());
  rack::Model *model = rack::Model::create<TModule, TPanel, TTag...>("DHE-Modules", slug, name, tags...);
  plugin->addModel(model);
}

void init(rack::Plugin *p) {
  plugin = p;

  plugin->slug = "DHE-Modules";
  plugin->version = TOSTRING(VERSION);

  registerModel<DHE::Blossom, DHE::BlossomPanel>("Blossom", rack::LFO_TAG);
  registerModel<DHE::BoosterStage, DHE::BoosterStagePanel>("Booster Stage", rack::ENVELOPE_GENERATOR_TAG);
  registerModel<DHE::Cubic, DHE::CubicPanel>("Cubic", rack::FUNCTION_GENERATOR_TAG);
  registerModel<DHE::Func, DHE::FuncPanel>("Func", rack::UTILITY_TAG);
  registerModel<DHE::Func6, DHE::Func6Panel>("Func 6", rack::UTILITY_TAG);
  registerModel<DHE::Hostage, DHE::HostagePanel>("Hostage", rack::ENVELOPE_GENERATOR_TAG);
  registerModel<DHE::Stage, DHE::StagePanel>("Stage", rack::ENVELOPE_GENERATOR_TAG);
  registerModel<DHE::Ranger, DHE::RangerPanel>("Ranger", rack::UTILITY_TAG);
  registerModel<DHE::Swave, DHE::SwavePanel>("Swave", rack::WAVESHAPER_TAG);
  registerModel<DHE::Tapers, DHE::TapersPanel>("Tapers", rack::UTILITY_TAG);
  registerModel<DHE::Upstage, DHE::UpstagePanel>("Upstage", rack::ENVELOPE_GENERATOR_TAG);

  plugin->addModel(modelXycloid);
}
