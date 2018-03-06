#pragma once

#include "module-widget.h"

namespace DHE {

struct SnobWidget : public ModuleWidget {
  explicit SnobWidget(rack::Module *module);
};
}