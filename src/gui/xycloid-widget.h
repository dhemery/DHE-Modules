#pragma once

#include "module-widget.h"

namespace DHE {

struct XycloidWidget : public ModuleWidget {
  explicit XycloidWidget(rack::Module *module);
  virtual void appendContextMenu(rack::Menu *menu) override;
};
}
