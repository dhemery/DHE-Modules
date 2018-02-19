#pragma once

#include "widget.h"

namespace DHE {

struct BoosterStageWidget : public Widget {
  explicit BoosterStageWidget(rack::Module *module);
};
}