#ifndef DHE_MODULES_MODULES_CONTROLS_H
#define DHE_MODULES_MODULES_CONTROLS_H

#include <util/interval.h>
#include <engine.hpp>
namespace DHE {

template <typename TOwner>
struct LevelControl {
  LevelControl(const TOwner *owner, int knob) : owner{*owner}, knob{owner->params[knob]} {}

  virtual float operator()() const {
    return UNIPOLAR_CV.scale(knob.value);
  }

  const TOwner &owner;
  const rack::Param &knob;
};

template <typename TOwner>
struct CVLevelControl : public LevelControl<TOwner> {
  CVLevelControl(const TOwner *owner, int knob, int cv)
      : LevelControl<TOwner>{owner, knob},
        cv{owner->inputs[cv]} {}

  float operator()() const override {
    auto modulation = cv.active ? UNIPOLAR_CV.normalize(cv.value) : 1.f;
    return UNIPOLAR_CV.scale(this->knob.value * modulation);
  }

  const rack::Input &cv;
};

template <typename TOwner>
struct ScalableLevelControl : public CVLevelControl<TOwner> {
  ScalableLevelControl(const TOwner *owner, int knob, int cv, int scale_switch, Interval low_scale, Interval high_scale)
      : CVLevelControl<TOwner>{owner, knob, cv},
        scale_switch{owner->params[scale_switch]},
        low_scale{low_scale},
        high_scale{high_scale}{}

  float operator()() const override {
    auto scale = scale_switch.value < 0.2f ? low_scale : high_scale;
    auto modulation = BIPOLAR_NORMAL.scale(this->cv.active ? scale.normalize(this->cv.value) : 1.f);
    auto value = BIPOLAR_NORMAL.scale(this->knob.value);
    auto modulated = value * modulation;
    return scale.clamp(scale.scale(BIPOLAR_NORMAL.normalize(modulated)));
  }

  const rack::Param &scale_switch;
  const Interval low_scale;
  const Interval high_scale;
};

}

#endif
