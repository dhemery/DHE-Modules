#pragma once

#include "module-widget.h"

namespace DHE {

struct CubicWidget : public ModuleWidget {
  explicit CubicWidget(rack::Module *module);
};
}