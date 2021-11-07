#pragma once

#include "control-ids.h"

#include "components/cxmath.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/common-inputs.h"
#include "signals/gain.h"
#include "signals/levels.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    Knob::config<Attenuator>(this, Param::Level, "Level", 100.F);
    configInput(Input::LevelCv, "Level CV");
    Knob::config<Attenuverter>(this, Param::LevelAv, "Level CV gain", 0.F);

    auto *ccw_limit_knob =
        Knob::config<Unipolar>(this, Param::CcwLimit, "CCW limit", 5.F);
    auto update_ccw_limit_knob_range = [ccw_limit_knob](Range r) {
      ccw_limit_knob->set_display_range(r);
    };
    Picker::config<Levels>(this, Param::CcwLimitRange, "CCW limit range",
                           Levels::Unipolar)
        ->on_change(update_ccw_limit_knob_range);
    configInput(Input::CcwLimitCv, "CCW limit CV");
    Knob::config<Attenuverter>(this, Param::CcwLimitAv, "CCW limit CV gain",
                               0.F);

    auto *cw_limit_knob =
        Knob::config<Unipolar>(this, Param::CwLimit, "CW limit", 5.F);
    auto update_cw_limit_knob_range = [cw_limit_knob](Range r) {
      cw_limit_knob->set_display_range(r);
    };
    Picker::config<Levels>(this, Param::CwLimitRange, "CW limit range",
                           Levels::Unipolar)
        ->on_change(update_cw_limit_knob_range);
    configInput(Input::CwLimitCv, "CW limit CV");
    Knob::config<Attenuverter>(this, Param::CwLimitAv, "CW limit CV gain", 0.F);

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
