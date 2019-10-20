#pragma once

#include "Func.h"
#include "Panel.h"

namespace dhe {
class FuncPanel : public Panel<FuncPanel> {
public:
  explicit FuncPanel(Func *func);
  static constexpr auto moduleSlug = "func";
  static constexpr auto hp = 3;
};
} // namespace dhe
