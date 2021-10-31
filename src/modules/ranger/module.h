#pragma once

#include "control-ids.h"

#include "components/cxmath.h"
#include "params/level-config.h"
#include "params/presets.h"
#include "signals/common-inputs.h"
#include "signals/levels.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    Attenuator::config(this, Param::Level, "Level");
    configInput(Input::LevelCv, "Level CV");
    Attenuverter::config(this, Param::LevelAv, "Level CV gain");

    auto *ccw_limit_knob =
        LevelKnob::config(this, Param::CcwLimit, "CCW limit");
    auto set_ccw_limit_range = [ccw_limit_knob](Range r) {
      ccw_limit_knob->set_range(r);
    };
    LevelSwitch::config(this, Param::CcwLimitRange, "CCW limit range",
                        Levels::Bipolar)
        ->set_action(set_ccw_limit_range);
    configInput(Input::CcwLimitCv, "CCW limit CV");
    Attenuverter::config(this, Param::CcwLimitAv, "CCW limit CV gain");

    auto *cw_limit_knob = LevelKnob::config(this, Param::CwLimit, "CW limit");
    auto set_cw_limit_range = [cw_limit_knob](Range r) {
      cw_limit_knob->set_range(r);
    };
    LevelSwitch::config(this, Param::CwLimitRange, "CW limit range", 0)
        ->set_action(set_cw_limit_range);
    configInput(Input::CwLimitCv, "CW limit CV");
    Attenuverter::config(this, Param::CwLimitAv, "CW limit CV gain");

    configOutput(Output::Main, "Ranger");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const output_voltage = cx::scale(level(), ccw_limit(), cw_limit());
    outputs[Output::Main].setVoltage(output_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level() const -> float {
    return rotation(params[Param::Level], inputs[Input::LevelCv],
                    params[Param::LevelAv]);
  }

  auto limit(int knob, int cv, int av) const -> float {
    return value_of(params[knob]) +
           voltage_at(inputs[cv]) * value_of(params[av]);
  }

  auto ccw_limit() const -> float {
    return limit(Param::CcwLimit, Input::CcwLimitCv, Param::CcwLimitAv);
  }

  auto cw_limit() const -> float {
    return limit(Param::CwLimit, Input::CwLimitCv, Param::CwLimitAv);
  }
};
} // namespace ranger
} // namespace dhe
