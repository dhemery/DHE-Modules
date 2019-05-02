#include <algorithm>

#include "rack.hpp"

#include "plugin/Plugin.hpp"

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

rack::plugin::Plugin *pluginInstance;

void init(rack::plugin::Plugin *p) {
    pluginInstance = p;


    p->addModel(rack::createModel<DHE::Blossom, DHE::BlossomPanel>("Blossom"));
    p->addModel(rack::createModel<DHE::BoosterStage, DHE::BoosterStagePanel>("BoosterStage"));
    p->addModel(rack::createModel<DHE::Cubic, DHE::CubicPanel>("Cubic"));
    p->addModel(rack::createModel<DHE::Func, DHE::FuncPanel>("Func"));
    p->addModel(rack::createModel<DHE::Func6, DHE::Func6Panel>("Func 6"));
    p->addModel(rack::createModel<DHE::Hostage, DHE::HostagePanel>("Hostage"));
    p->addModel(rack::createModel<DHE::Stage, DHE::StagePanel>("Stage"));
    p->addModel(rack::createModel<DHE::Ranger, DHE::RangerPanel>("Ranger"));
    p->addModel(rack::createModel<DHE::Swave, DHE::SwavePanel>("Swave"));
    p->addModel(rack::createModel<DHE::Tapers, DHE::TapersPanel>("Tapers"));
    p->addModel(rack::createModel<DHE::Upstage, DHE::UpstagePanel>("Upstage"));
    p->addModel(rack::createModel<DHE::Xycloid, DHE::XycloidPanel>("Xycloid"));
}
