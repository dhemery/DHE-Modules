#include <array>
#include <math.h>

#include "dhe-modules.h"
#include "display/controls.h"
#include "display/panel.h"
#include "util/rotation.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

class BloomerRotor {
public:
  void advance(float delta, float offset = 0.f) {
    this->offset = offset;
    phase += delta;
    phase -= std::trunc(phase);
  }

  auto angle() const -> float { return two_pi*(phase + offset); }
  auto radius() const -> float { return std::sin(angle()); }

private:
  float const two_pi{2.f*std::acos(-1.f)};
  float phase{0.f};
  float offset{0.f};
};

class Bloomer : public rack::Module {
public:
  Bloomer() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto spin_rate = spin();
    auto bounce_ratio = bounce();
    auto bounce_depth = depth();

    spinner.advance(spin_rate, 0.f);
    bouncer.advance(spin_rate * bounce_ratio, phase());

    auto angle = spinner.angle();
    auto radius = (1.f - bounce_depth) + bounce_depth * bouncer.radius();

    auto x = radius * std::cos(angle);
    auto y = radius * std::sin(angle);

    outputs[X_OUT].value = 5.f*x_gain_in()*(x + x_offset());
    outputs[Y_OUT].value = 5.f*y_gain_in()*(y + y_offset());
  }

  enum ParameterIds {
    SPIN_KNOB,
    SPIN_AV,
    BOUNCE_KNOB,
    BOUNCE_AV,
    DEPTH_KNOB,
    DEPTH_AV,
    PHASE_KNOB,
    PHASE_AV,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    SPIN_CV,
    BOUNCE_CV,
    DEPTH_CV,
    PHASE_CV,
    X_GAIN_CV,
    Y_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds { X_OUT, Y_OUT, OUTPUT_COUNT };

private:
  auto modulated(const ParameterIds &knob_param, const InputIds &cv_input) const
  -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Rotation::modulated(rotation, cv);
  }

  auto modulated(const ParameterIds &knob_param, const InputIds &cv_input,
                 const ParameterIds &av_param) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    auto av = params[av_param].value;
    return Rotation::modulated(rotation, cv, av);
  }

  auto offset(int param) const -> float {
    auto is_uni = params[param].value > 0.5f;
    return is_uni ? 1.f : 0.f;
  }

  auto bounce() const -> float {
    static constexpr auto bounce_range = Range{1.f, 17.f};
    auto rotation = modulated(BOUNCE_KNOB, BOUNCE_CV, BOUNCE_AV);
    return bounce_range.scale(rotation);
  }

  auto spin() const -> float {
    static constexpr auto spin_range = Range{-1.f, 1.f};
    auto rotation = modulated(SPIN_KNOB, SPIN_CV, SPIN_AV);
    auto scaled = spin_range.scale(rotation);
    auto tapered = Sigmoid::inverse(scaled, speed_curvature);
    return -10.f*tapered*rack::engineGetSampleTime();
  }

  auto depth() const -> float {
    static constexpr auto depth_range = Range{0.f, 1.f};
    auto rotation = modulated(DEPTH_KNOB, DEPTH_CV, DEPTH_AV);
    return depth_range.clamp(rotation);
  }

  auto phase() const -> float {
    static constexpr auto phase_range = Range{0.f, 1.f};
    auto rotation = modulated(PHASE_KNOB, PHASE_CV, PHASE_AV);
    return phase_range.clamp(rotation);
  }

  auto x_offset() const -> float { return offset(X_RANGE_SWITCH); }

  auto x_gain_in() const -> float {
    return Rotation::gain_multiplier(modulated(X_GAIN_KNOB, X_GAIN_CV));
  }

  auto y_gain_in() const -> float {
    return Rotation::gain_multiplier(modulated(Y_GAIN_KNOB, Y_GAIN_CV));
  }

  auto y_offset() const -> float { return offset(Y_RANGE_SWITCH); }

  static constexpr auto speed_curvature = 0.8f;

  BloomerRotor spinner{};
  BloomerRotor bouncer{};
};

class BloomerPanel : public Panel<BloomerPanel> {
public:
  explicit BloomerPanel(Bloomer *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = widget_right_edge/7.f;
    auto column_4 = widget_right_edge - column_1;
    auto column_2 = (column_4 - column_1)/3.f + column_1;
    auto column_3 = widget_right_edge - column_2;

    auto y = 25.f;
    auto dy = 18.5f;

    install(column_1, y, input(Bloomer::SPIN_CV));
    install(column_2, y, knob<TinyKnob>(Bloomer::SPIN_AV));
    install(column_3, y, knob<LargeKnob>(Bloomer::SPIN_KNOB, 0.65f));

    y += dy;
    install(column_1, y, input(Bloomer::BOUNCE_CV));
    install(column_2, y, knob<TinyKnob>(Bloomer::BOUNCE_AV));
    install(column_3, y, knob<LargeKnob>(Bloomer::BOUNCE_KNOB));

    y += dy;
    install(column_1, y, input(Bloomer::DEPTH_CV));
    install(column_2, y, knob<TinyKnob>(Bloomer::DEPTH_AV));
    install(column_3, y, knob<LargeKnob>(Bloomer::DEPTH_KNOB));

    y += dy;
    install(column_1, y, input(Bloomer::PHASE_CV));
    install(column_2, y, knob<TinyKnob>(Bloomer::PHASE_AV));
    install(column_3, y, knob<LargeKnob>(Bloomer::PHASE_KNOB));

    y = 97.f;
    dy = 15.f;
    const auto output_port_offset = 1.25f;

    auto default_gain = Rotation::gain_range.normalize(1.f);

    install(column_1, y, input(Bloomer::X_GAIN_CV));
    install(column_2, y, knob<SmallKnob>(Bloomer::X_GAIN_KNOB, default_gain));
    install(column_3, y, toggle<2>(Bloomer::X_RANGE_SWITCH, 0));
    install(column_4, y + output_port_offset, output(Bloomer::X_OUT));

    y += dy;
    install(column_1, y, input(Bloomer::Y_GAIN_CV));
    install(column_2, y, knob<SmallKnob>(Bloomer::Y_GAIN_KNOB, default_gain));
    install(column_3, y, toggle<2>(Bloomer::Y_RANGE_SWITCH, 0));
    install(column_4, y + output_port_offset, output(Bloomer::Y_OUT));
  }

  static constexpr auto module_slug = "bloomer";

private:
  static constexpr auto hp = 11;
};

} // namespace DHE
rack::Model *modelBloomer =
    rack::Model::create<DHE::Bloomer, DHE::BloomerPanel>(
        "DHE-Modules", "Bloomer", "Bloomer", rack::LFO_TAG);
