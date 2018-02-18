#ifndef DHE_MODULES_MODULES_UPSTAGE_WIDGET_H
#define DHE_MODULES_MODULES_UPSTAGE_WIDGET_H

#include "widget.h"

namespace DHE {

struct UpstageWidget : public Widget {
  explicit UpstageWidget(rack::Module *module);
};
}
#endif