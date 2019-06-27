#include "panels/FlogicPanel.h"

namespace dhe {

FlogicPanel::FlogicPanel(Flogic *flogic) : Panel{flogic, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const top = hp2mm(4.F);
  auto const dy = hp2mm(3.5F);

  auto y = top + 0.F * dy;
  input(left, y, Flogic::AInput);
  button<ToggleButton>(lc, y, Flogic::NotAButton);
  button<ToggleButton>(rc, y, Flogic::NotBButton);
  input(right, y, Flogic::BInput);

  y = top + 1.F * dy;
  output(left, y, Flogic::ZAndOutput);
  output(lc, y, Flogic::ZNandOutput);
  output(rc, y, Flogic::HNandOutput);
  output(right, y, Flogic::HAndOutput);

  y = top + 2.F * dy;
  output(left, y, Flogic::ZOrOutput);
  output(lc, y, Flogic::ZNorOutput);
  output(rc, y, Flogic::HXnorOutput);
  output(right, y, Flogic::HXorOutput);

  y = top + 3.F * dy;
  output(left, y, Flogic::ZXorOutput);
  output(lc, y, Flogic::ZXnorOutput);
  output(rc, y, Flogic::HXnorOutput);
  output(right, y, Flogic::HXorOutput);

  y = top + 4.F * dy;
  output(left, y, Flogic::ZImplicationOutput);
  output(lc, y, Flogic::ZNonimplicationOutput);
  output(rc, y, Flogic::HNonimplicationOutput);
  output(right, y, Flogic::HImplicationOutput);

  y = top + 5.F * dy;
  output(left, y, Flogic::ZConverseImplicationOutput);
  output(lc, y, Flogic::ZConverseNonimplicationOutput);
  output(rc, y, Flogic::HConverseNonimplicationOutput);
  output(right, y, Flogic::HConverseImplicationOutput);
}
} // namespace dhe
