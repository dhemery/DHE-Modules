#pragma once

#include "widget.h"

namespace DHE {

struct SwaveWidget : public Widget {
  explicit SwaveWidget(rack::Module *module);
};
}