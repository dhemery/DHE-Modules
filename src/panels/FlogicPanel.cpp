#include "panels/FlogicPanel.h"

namespace dhe {

FlogicPanel::FlogicPanel(Flogic *gator) : Panel{gator, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const dy = hp2mm(3.F);
  auto top = hp2mm(4.5F);

  auto y = top + 0.F * dy;
  input(left, y, Flogic::AInputs + 0);
  button<ToggleButton>(lc, y, Flogic::ANotButtons + 0);
  button<ToggleButton>(rc, y, Flogic::BNotButtons + 0);
  input(right, y, Flogic::BInputs + 0);

  y = top + 1.F * dy;
  output(left, y, Flogic::ZAndOutputs + 0);
  output(lc, y, Flogic::ZNandOutputs + 0);
  output(rc, y, Flogic::ZNorOutputs + 0);
  output(right, y, Flogic::ZOrOutputs + 0);

  y = top + 2.F * dy;
  output(left, y, Flogic::PAndOutputs + 0);
  output(lc, y, Flogic::PNandOutputs + 0);
  output(rc, y, Flogic::PNorOutputs + 0);
  output(right, y, Flogic::POrOutputs + 0);

  top += hp2mm(10.F);

  y = top + 0.F * dy;
  input(left, y, Flogic::AInputs + 1);
  button<ToggleButton>(lc, y, Flogic::ANotButtons + 1);
  button<ToggleButton>(rc, y, Flogic::BNotButtons + 1);
  input(right, y, Flogic::BInputs + 1);

  y = top + 1.F * dy;
  output(left, y, Flogic::ZAndOutputs + 1);
  output(lc, y, Flogic::ZNandOutputs + 1);
  output(rc, y, Flogic::ZNorOutputs + 1);
  output(right, y, Flogic::ZOrOutputs + 1);

  y = top + 2.F * dy;
  output(left, y, Flogic::PAndOutputs + 1);
  output(lc, y, Flogic::PNandOutputs + 1);
  output(rc, y, Flogic::PNorOutputs + 1);
  output(right, y, Flogic::POrOutputs + 1);
}
} // namespace dhe
