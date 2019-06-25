#include "panels/ZadehPanel.h"

namespace dhe {

ZadehPanel::ZadehPanel(Zadeh *zadeh) : Panel{zadeh, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto top = hp2mm(4.F);
  auto const dy = hp2mm(3.F);

  auto y = top + 0.F * dy;
  input(left, y, Zadeh::AInputs + 0);
  button<ToggleButton>(lc, y, Zadeh::NotAButtons + 0);
  button<ToggleButton>(rc, y, Zadeh::NotAButtons + 1);
  input(right, y, Zadeh::AInputs + 1);

  y = top + 1.F * dy;
  input(left, y, Zadeh::BInputs + 0);
  button<ToggleButton>(lc, y, Zadeh::NotBButtons + 0);
  button<ToggleButton>(rc, y, Zadeh::NotBButtons + 1);
  input(right, y, Zadeh::BInputs + 1);

  y = top + 2.F * dy;
  output(left, y, Zadeh::AndOutputs + 0);
  output(lc, y, Zadeh::NandOutputs + 0);
  output(rc, y, Zadeh::NandOutputs + 1);
  output(right, y, Zadeh::AndOutputs + 1);

  y = top + 3.F * dy;
  output(left, y, Zadeh::OrOutputs + 0);
  output(lc, y, Zadeh::NorOutputs + 0);
  output(rc, y, Zadeh::NorOutputs + 1);
  output(right, y, Zadeh::OrOutputs + 1);

  y = top + 4.F * dy;
  output(left, y, Zadeh::XorOutputs + 0);
  output(lc, y, Zadeh::XnorOutputs + 0);
  output(rc, y, Zadeh::XnorOutputs + 1);
  output(right, y, Zadeh::XorOutputs + 1);

  y = top + 5.F * dy;
  output(left, y, Zadeh::AImpliesBOutputs + 0);
  output(lc, y, Zadeh::ANotImpliesBOutputs + 0);
  output(rc, y, Zadeh::ANotImpliesBOutputs + 1);
  output(right, y, Zadeh::AImpliesBOutputs + 1);

  y = top + 6.F * dy;
  output(left, y, Zadeh::BImpliesAOutputs + 0);
  output(lc, y, Zadeh::BNotImpliesAOutputs + 0);
  output(rc, y, Zadeh::BNotImpliesAOutputs + 1);
  output(right, y, Zadeh::BImpliesAOutputs + 1);
}
} // namespace dhe
