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
    auto modulation = this->cv.active ? cv.value : 0.f;
    return UNIPOLAR_CV.scale(this->knob.value) + modulation;
  }

  const rack::Input &cv;
};

template<typename TOwner>
struct ScalableLevelControl : public ModulatedLevelControl<TOwner> {
  ScalableLevelControl(const TOwner *owner, int knob, int cv, int scale_switch)
      : ModulatedLevelControl<TOwner>{owner, knob, cv},
        scale_switch{owner->params[scale_switch]} {}

  float operator()() const override {
    auto modulation = this->cv.active ? this->cv.value : 0.f;
    auto range = scale_switch.value < 0.2f ? BIPOLAR_CV : UNIPOLAR_CV;
    return range.scale(this->knob.value) + modulation;
  }

  const rack::Param &scale_switch;
};

}

#endif
