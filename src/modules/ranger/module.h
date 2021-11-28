#pragma once

#include "control-ids.h"

#include "components/cxmath.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/linear-signals.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {

struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    PercentageKnob::config(this, ParamId::Level, "Level", 50.F);
    configInput(InputId::LevelCv, "Level CV");
    AttenuverterKnob::config(this, ParamId::LevelAv, "Level CV gain");

    auto *ccw_limit_knob =
        VoltageKnob::config(this, ParamId::CcwLimit, "CCW limit", -5.F);
    VoltageRangeSwitch::config(this, ParamId::CcwLimitRange, "CCW limit range",
                               VoltageRangeId::Bipolar)
        ->add_knob(ccw_limit_knob);
    configInput(InputId::CcwLimitCv, "CCW limit CV");
    AttenuverterKnob::config(this, ParamId::CcwLimitAv, "CCW limit CV gain");

    auto *cw_limit_knob =
        VoltageKnob::config(this, ParamId::CwLimit, "CW limit", 5.F);
    VoltageRangeSwitch::config(this, ParamId::CwLimitRange, "CW limit range",
                               VoltageRangeId::Bipolar)
        ->add_knob(cw_limit_knob);
    configInput(InputId::CwLimitCv, "CW limit CV");
    AttenuverterKnob::config(this, ParamId::CwLimitAv, "CW limit CV gain");

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
