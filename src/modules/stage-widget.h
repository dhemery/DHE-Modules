#ifndef DHE_MODULES_MODULES_STAGE_WIDGET_H
#define DHE_MODULES_MODULES_STAGE_WIDGET_H

#include "widget.h"

namespace DHE {

struct StageWidget : public Widget {
  StageWidget(rack::Module *module);
};
}
#endif