#pragma once

#include <plugin/dhe-modules.h>
#include "module-widget.h"

namespace DHE {

struct UpstageWidget : public ModuleWidget {
  explicit UpstageWidget(rack::Module *module);
};
}