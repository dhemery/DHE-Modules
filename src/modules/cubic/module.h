#pragma once

#include "coefficient.h"
#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "params/presets.h"

#include "rack.hpp"

namespace dhe {

namespace cubic {

struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    Knob::config<Coefficient>(this, ParamId::ACoefficient,
                              "X cubed coefficient");
    configInput(InputId::ACoefficientCv, "X cubed coefficient CV");

    Knob::config<Coefficient>(this, ParamId::BCoefficient,
                              "X squared coefficient");
    configInput(InputId::BCoefficientCv, "X squared coefficient CV");

    Knob::config<Coefficient>(this, ParamId::CCoefficient, "X coefficient");
    configInput(InputId::CCoefficientCv, "X coefficient CV");

    Knob::config<Coefficient>(this, ParamId::DCoefficient,
                              "Constant coefficient");
    configInput(InputId::DCoefficientCv, "Constant coefficient CV");

    Knob::config<Gain>(this, ParamId::InputGain, "InPort gain");
    configInput(InputId::InputGainCv, "InPort gain CV");

    Knob::config<Gain>(this, ParamId::OutputGain, "OutPort gain");
    configInput(InputId::OutputGainCv, "OutPort gain CV");

    configInput(InputId::Cubic, "Module");
    configOutput(OutputId::Cubic, "Module");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto a = coefficient(ParamId::ACoefficient, InputId::ACoefficientCv);
    auto b = coefficient(ParamId::BCoefficient, InputId::BCoefficientCv);
    auto c = coefficient(ParamId::CCoefficient, InputId::CCoefficientCv);
    auto d = coefficient(ParamId::DCoefficient, InputId::DCoefficientCv);
    auto input_gain = gain(ParamId::InputGain, InputId::InputGainCv);
    auto output_gain = gain(ParamId::OutputGain, InputId::OutputGainCv);

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
    auto const rotation = rotation_of(params[knob_id], inputs[cv_id]);
    return Coefficient::scale(rotation);
  }

  auto gain(int knob_id, int cv_id) const -> float {
    auto const rotation = rotation_of(params[knob_id], inputs[cv_id]);
    return Gain::scale(rotation);
  }

  auto main_in() const -> float { return voltage_at(inputs[InputId::Cubic]); }

  void send_main_out(float voltage) {
    outputs[OutputId::Cubic].setVoltage(voltage);
  }
};
} // namespace cubic
} // namespace dhe
