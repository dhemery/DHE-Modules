#ifndef DHE_MODULES_MODULES_BOOSTER_STAGE_WIDGET_H
#define DHE_MODULES_MODULES_BOOSTER_STAGE_WIDGET_H

#include "widget.h"

namespace DHE {

struct BoosterStageWidget : public Widget {
  explicit BoosterStageWidget(rack::Module *module);
};
}
#endif