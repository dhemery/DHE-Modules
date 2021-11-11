#pragma once

#include "control-ids.h"

#include "components/cxmath.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/gain.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    Knob::config<Percentage>(this, Param::Level, "Level");
    configInput(Input::LevelCv, "Level CV");
    Knob::config<Attenuverter>(this, Param::LevelAv, "Level CV gain");

    auto *ccw_limit_knob =
        Knob::config<Voltages>(this, Param::CcwLimit, "CCW limit", 0.F);
    auto select_ccw_limit_level_range = [ccw_limit_knob](int range_index) {
      ccw_limit_knob->mapper().select_range(range_index);
    };
    Switch::config<Voltages>(this, Param::CcwLimitRange, "CCW limit range",
                             Voltages::Bipolar)
        ->on_change(select_ccw_limit_level_range);
    configInput(Input::CcwLimitCv, "CCW limit CV");
    Knob::config<Attenuverter>(this, Param::CcwLimitAv, "CCW limit CV gain");

    auto *cw_limit_knob =
        Knob::config<Voltages>(this, Param::CwLimit, "CW limit", 1.F);
    auto select_cw_limit_level_range = [cw_limit_knob](int range_index) {
      cw_limit_knob->mapper().select_range(range_index);
    };
    Switch::config<Voltages>(this, Param::CwLimitRange, "CW limit range",
                             Voltages::Bipolar)
        ->on_change(select_cw_limit_level_range);
    configInput(Input::CwLimitCv, "CW limit CV");
    Knob::config<Attenuverter>(this, Param::CwLimitAv, "CW limit CV gain");

    configOutput(Output::Main, "Ranger");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[Output::Main].setVoltage(level());
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level() const -> float {
    return cx::scale(rotation(params[Param::Level], inputs[Input::LevelCv],
                              params[Param::LevelAv]),
                     ccw_limit(), cw_limit());
  }

  inline auto limit(int knob, int cv, int av, int range_selection) const
      -> float {
    return Voltages::volts(rotation(params[knob], inputs[cv], params[av]),
                           position_of(params[range_selection]));
  }

  auto ccw_limit() const -> float {
    return limit(Param::CcwLimit, Input::CcwLimitCv, Param::CcwLimitAv,
                 Param::CcwLimitRange);
  }

  auto cw_limit() const -> float {
    return limit(Param::CwLimit, Input::CwLimitCv, Param::CwLimitAv,
                 Param::CwLimitRange);
  }
};
} // namespace ranger
} // namespace dhe
