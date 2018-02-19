#pragma once

#include "widget.h"

namespace DHE {

struct StageWidget : public Widget {
  explicit StageWidget(rack::Module *module);
};
}