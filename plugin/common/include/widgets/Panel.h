#pragma once

#include "Buttons.h"
#include "CommonWidgets.h"
#include "Dimensions.h"
#include "Jacks.h"
#include "Knobs.h"
#include "PanelAssets.h"
#include "Screws.h"
#include "Toggles.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

template <typename P> class Panel : public rack::app::ModuleWidget {
protected:
  template <template <typename> class K> auto knob(float x, float y, int index) -> K<P> * {
    return installParam<K<P>>(this, module, x, y, index);
  }

  template <template <typename> class B = Button> auto button(float x, float y, int index) -> B<P> * {
    return installParam<B<P>>(this, module, x, y, index);
  }

  template <template <typename> class C> auto toggle(float x, float y, int index) -> C<P> * {
    return installParam<C<P>>(this, module, x, y, index);
  }

  template <template <typename, int> class C, int N> auto toggle(float x, float y, int index) -> C<P, N> * {
    return installParam<C<P, N>>(this, module, x, y, index);
  }

  template <int N> auto toggle(float x, float y, int index) -> Toggle<P, N> * {
    return installParam<Toggle<P, N>>(this, module, x, y, index);
  }

  // TODO: Inline the following functions

  template <typename Light> auto light(float x, float y, int index) -> Light * {
    return installLight<Light>(this, module, x, y, index);
  }

  auto input(float x, float y, int index) -> InputJack<P> * { return InputJack<P>::install(this, module, x, y, index); }

  auto output(float x, float y, int index) -> OutputJack<P> * {
    return OutputJack<P>::install(this, module, x, y, index);
  }
};
} // namespace dhe
