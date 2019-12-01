#pragma once

#include "Buttons.h"
#include "CommonWidgets.h"
#include "Toggles.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

template <typename P> class Panel : public rack::app::ModuleWidget {
protected:
//  template <template <typename> class C> auto toggle(float x, float y, int index) -> C<P> * {
//    return install<C<P>>(this, module, x, y, index);
//  }

//  template <template <typename, int> class C, int N> auto toggle(float x, float y, int index) -> C<P, N> * {
//    return install<C<P, N>>(this, module, x, y, index);
//  }
};
} // namespace dhe
