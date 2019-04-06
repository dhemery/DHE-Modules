#include "display/panel.h"

namespace DHE {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(rack::Module *cubic);
  static constexpr auto module_slug = "cubic";
  static constexpr auto hp = 5;
};
}