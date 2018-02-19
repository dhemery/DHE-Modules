#pragma once

#include "widget.h"

namespace DHE {

struct UpstageWidget : public Widget {
  explicit UpstageWidget(rack::Module *module);
};
}