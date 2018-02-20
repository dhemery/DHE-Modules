#pragma once

#include "widget.h"

namespace DHE {

struct SJWidget : public Widget {
  explicit SJWidget(rack::Module *module);
};
}