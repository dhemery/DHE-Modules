#pragma once

#include "CommonWidgets.h"
#include "Buttons.h"
#include "Toggles.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

template <typename P> class Panel : public rack::app::ModuleWidget {
protected:
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
};
} // namespace dhe
