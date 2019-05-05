#pragma once

#include "panel.h"
#include "modules/Func6.h"

namespace DHE {

class Func6Panel : public Panel<Func6Panel> {
public:
  explicit Func6Panel(Func6 *func6);
  static constexpr auto module_slug = "func-6";
  static constexpr auto hp = 12;
};
} // namespace DHE
