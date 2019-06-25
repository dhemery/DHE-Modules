#pragma once

#include "Panel.h"
#include "modules/Zadeh.h"

namespace dhe {

class ZadehPanel : public Panel<ZadehPanel> {
public:
  explicit ZadehPanel(Zadeh *zadeh);
  static constexpr auto moduleSlug = "zadeh";
  static constexpr auto hp = 9;
};
} // namespace dhe
