#include <algorithm>

#include "panels/BlossomPanel.h"
#include "panels/BoosterStagePanel.h"
#include "panels/CubicPanel.h"
#include "panels/Func6Panel.h"
#include "panels/FuncPanel.h"
#include "panels/HostagePanel.h"
#include "panels/RangerPanel.h"
#include "panels/StagePanel.h"
#include "panels/SwavePanel.h"
#include "panels/TapersPanel.h"
#include "panels/UpstagePanel.h"
#include "panels/XycloidPanel.h"

rack::plugin::Plugin *plugin;

template <typename TModule, typename TPanel>
void registerModel(const std::string &name) {
  auto slug = name;
  slug.erase(std::remove(slug.begin(), slug.end(), ' '), slug.end());
  rack::plugin::Model *model = rack::createModel<TModule, TPanel>(slug);
  plugin->addModel(model);
}

void init(rack::plugin::Plugin *p) {
  plugin = p;

  plugin->slug = "DHE-Modules";
  plugin->version = TOSTRING(VERSION);

  registerModel<DHE::Blossom, DHE::BlossomPanel>("Blossom");
  registerModel<DHE::BoosterStage, DHE::BoosterStagePanel>("Booster Stage");
  registerModel<DHE::Cubic, DHE::CubicPanel>("Cubic");
  registerModel<DHE::Func, DHE::FuncPanel>("Func");
  registerModel<DHE::Func6, DHE::Func6Panel>("Func 6");
  registerModel<DHE::Hostage, DHE::HostagePanel>("Hostage");
  registerModel<DHE::Stage, DHE::StagePanel>("Stage");
  registerModel<DHE::Ranger, DHE::RangerPanel>("Ranger");
  registerModel<DHE::Swave, DHE::SwavePanel>("Swave");
  registerModel<DHE::Tapers, DHE::TapersPanel>("Tapers");
  registerModel<DHE::Upstage, DHE::UpstagePanel>("Upstage");
  registerModel<DHE::Xycloid, DHE::XycloidPanel>("Xycloid");
}
