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
        Knob::config<VoltageRanges>(this, Param::CcwLimit, "CCW limit", 0.F);
    auto select_ccw_limit_range = [ccw_limit_knob](VoltageRangeId id) {
      ccw_limit_knob->mapper().select_range(id);
    };
    Switch::config<VoltageRanges>(this, Param::CcwLimitRange,
                                  "CCW limit select", VoltageRangeId::Bipolar)
        ->on_change(select_ccw_limit_range);
    configInput(Input::CcwLimitCv, "CCW limit CV");
    Knob::config<Attenuverter>(this, Param::CcwLimitAv, "CCW limit CV gain");

    auto *cw_limit_knob =
        Knob::config<VoltageRanges>(this, Param::CwLimit, "CW limit", 1.F);
    auto select_cw_limit_range = [cw_limit_knob](VoltageRangeId id) {
      cw_limit_knob->mapper().select_range(id);
    };
    Switch::config<VoltageRanges>(this, Param::CwLimitRange, "CW limit select",
                                  VoltageRangeId::Bipolar)
        ->on_change(select_cw_limit_range);
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
    auto const rotation = rotation_of(
        params[Param::Level], inputs[Input::LevelCv], params[Param::LevelAv]);
    return cx::scale(rotation, ccw_limit(), cw_limit());
  }

  inline auto limit(int knob, int cv, int av, int range) const -> float {
    auto const rotation = rotation_of(params[knob], inputs[cv], params[av]);
    auto const range_id = value_of<VoltageRangeId>(params[range]);
    return VoltageRanges::volts(rotation, range_id);
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
