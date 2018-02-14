#ifndef DHE_STAGE_UPSTAGE_WIDGET_H
#define DHE_STAGE_UPSTAGE_WIDGET_H

#include "widget.h"

namespace DHE {

struct UpstageWidget : public Widget {
  UpstageWidget(rack::Module *module);
};
}
#endif