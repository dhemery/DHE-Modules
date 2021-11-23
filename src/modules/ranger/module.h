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
    config(ParamId::Count, InputId::Count, OutputId::Count);

    Knob::config<Percentage>(this, ParamId::Level, "Level");
    configInput(InputId::LevelCv, "Level CV");
    Knob::config<Attenuverter>(this, ParamId::LevelAv, "Level CV gain");

    auto *ccw_limit_knob =
        Knob::config<Voltage>(this, ParamId::CcwLimit, "CCW limit", 0.F);
    auto select_ccw_limit_range = [ccw_limit_knob](VoltageRangeId id) {
      ccw_limit_knob->mapper().select_range(id);
    };
    Switch::config<VoltageRanges>(this, ParamId::CcwLimitRange,
                                  "CCW limit range", VoltageRangeId::Bipolar)
        ->on_change(select_ccw_limit_range);
    configInput(InputId::CcwLimitCv, "CCW limit CV");
    Knob::config<Attenuverter>(this, ParamId::CcwLimitAv, "CCW limit CV gain");

    auto *cw_limit_knob =
        Knob::config<Voltage>(this, ParamId::CwLimit, "CW limit", 1.F);
    auto select_cw_limit_range = [cw_limit_knob](VoltageRangeId id) {
      cw_limit_knob->mapper().select_range(id);
    };
    Switch::config<VoltageRanges>(this, ParamId::CwLimitRange, "CW limit range",
                                  VoltageRangeId::Bipolar)
        ->on_change(select_cw_limit_range);
    configInput(InputId::CwLimitCv, "CW limit CV");
    Knob::config<Attenuverter>(this, ParamId::CwLimitAv, "CW limit CV gain");

    configOutput(OutputId::Main, "Ranger");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[OutputId::Main].setVoltage(level());
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level() const -> float {
    auto const rotation =
        rotation_of(params[ParamId::Level], inputs[InputId::LevelCv],
                    params[ParamId::LevelAv]);
    return cx::scale(rotation, ccw_limit(), cw_limit());
  }

  inline auto limit(int knob, int cv, int av, int range) const -> float {
    auto const rotation = rotation_of(params[knob], inputs[cv], params[av]);
    auto const range_id = value_of<VoltageRangeId>(params[range]);
    return Voltage::scale(rotation, range_id);
  }

  auto ccw_limit() const -> float {
    return limit(ParamId::CcwLimit, InputId::CcwLimitCv, ParamId::CcwLimitAv,
                 ParamId::CcwLimitRange);
  }

  auto cw_limit() const -> float {
    return limit(ParamId::CwLimit, InputId::CwLimitCv, ParamId::CwLimitAv,
                 ParamId::CwLimitRange);
  }
};
} // namespace ranger
} // namespace dhe
