#ifndef DHE_MODULES_MODULES_CONTROLS_H
#define DHE_MODULES_MODULES_CONTROLS_H

#include <util/interval.h>
#include <engine.hpp>
namespace DHE {

template<typename TOwner>
struct LevelControl {
  LevelControl(const TOwner *owner, int knob) : owner{*owner}, knob{owner->params[knob]} {}

  virtual float operator()() const {
    return UNIPOLAR_CV.scale(knob.value);
  }

  const TOwner &owner;
  const rack::Param &knob;
};

template<typename TOwner>
struct ModulatedLevelControl : public LevelControl<TOwner> {
  ModulatedLevelControl(const TOwner *owner, int knob, int cv)
      : LevelControl<TOwner>{owner, knob},
        cv{owner->inputs[cv]} {}

  float operator()() const override {
    if (this->cv.active) {
      return (this->cv.value + UNIPOLAR_CV.center()) * this->knob.value;
    }
    return UNIPOLAR_CV.scale(this->knob.value);
  }

  const rack::Input &cv;
};

template<typename TOwner>
struct ScalableLevelControl : public ModulatedLevelControl<TOwner> {
  ScalableLevelControl(const TOwner *owner, int knob, int cv, int scale_switch)
      : ModulatedLevelControl<TOwner>{owner, knob, cv},
        scale_switch{owner->params[scale_switch]} {}

  float operator()() const override {
    auto range = scale_switch.value < 0.2f ? BIPOLAR_CV : UNIPOLAR_CV;

    if (this->cv.active) {
      return (this->cv.value + range.center()) * this->knob.value;
    }
    return range.scale(this->knob.value);
  }

  const rack::Param &scale_switch;
};

}

#endif
