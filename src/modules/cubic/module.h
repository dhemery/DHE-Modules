#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "params/presets.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace cubic {

struct Coefficient {
  static auto constexpr range() -> Range { return Range{-2.F, 2.F}; }
  static auto constexpr display_range() -> Range { return range(); }
  static auto constexpr unit = "";

  static inline auto value(float rotation) -> float {
    return range().scale(rotation);
  }
};

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    Knob::config<Coefficient>(this, Param::ACoefficient, "X cubed coefficient",
                              0.5F);
    configInput(Input::ACoefficientCv, "X cubed coefficient CV");

    Knob::config<Coefficient>(this, Param::BCoefficient,
                              "X squared coefficient", 0.5F);
    configInput(Input::BCoefficientCv, "X squared coefficient CV");

    Knob::config<Coefficient>(this, Param::CCoefficient, "X coefficient", 0.5F);
    configInput(Input::CCoefficientCv, "X coefficient CV");

    Knob::config<Coefficient>(this, Param::DCoefficient, "Constant coefficient",
                              0.5F);
    configInput(Input::DCoefficientCv, "Constant coefficient CV");

    Knob::config<Gain>(this, Param::InputGain, "InPort gain", 0.5F);
    configInput(Input::InputGainCv, "InPort gain CV");

    Knob::config<Gain>(this, Param::OutputGain, "OutPort gain", 0.5F);
    configInput(Input::OutputGainCv, "OutPort gain CV");

    configInput(Input::Cubic, "Module");
    configOutput(Output::Cubic, "Module");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto a = coefficient(Param::ACoefficient, Input::ACoefficientCv);
    auto b = coefficient(Param::BCoefficient, Input::BCoefficientCv);
    auto c = coefficient(Param::CCoefficient, Input::CCoefficientCv);
    auto d = coefficient(Param::DCoefficient, Input::DCoefficientCv);
    auto input_gain = gain(Param::InputGain, Input::InputGainCv);
    auto output_gain = gain(Param::OutputGain, Input::OutputGainCv);

    auto x = input_gain * main_in() * 0.2F;
    auto x2 = x * x;
    auto x3 = x2 * x;
    auto y = a * x3 + b * x2 + c * x + d;
    auto output_voltage = output_gain * y * 5.F;
    send_main_out(output_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto coefficient(int knob_id, int cv_id) const -> float {
    return Coefficient::value(rotation(params[knob_id], inputs[cv_id]));
  }

  auto gain(int knob_id, int cv_id) const -> float {
    return Gain::value(rotation(params[knob_id], inputs[cv_id]));
  }

  auto main_in() const -> float { return voltage_at(inputs[Input::Cubic]); }

  void send_main_out(float voltage) {
    outputs[Output::Cubic].setVoltage(voltage);
  }
};
} // namespace cubic
} // namespace dhe
