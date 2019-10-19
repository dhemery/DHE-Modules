#pragma once

#include "Func6.h"
#include "modules/Panel.h"

namespace dhe {

class Func6Panel : public Panel<Func6Panel> {
public:
  explicit Func6Panel(Func6 *func6);
  static constexpr auto moduleSlug = "func-6";
  static constexpr auto hp = 12;
};
} // namespace dhe
