#include <array>

#include "dhe-modules.h"
#include "display/controls.h"
#include "display/panel.h"
#include "util/rotation.h"
#include "util/rotor.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

class Blossom : public rack::Module {
public:
  Blossom() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto spin_rate = spin();
    auto bounce_ratio = is_bounce_free() ? bounce() : std::round(bounce());
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
    BOUNCE_LOCK_SWITCH,
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

  auto is_bounce_free() const -> bool {
    return params[BOUNCE_LOCK_SWITCH].value > 0.1f;
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

  Rotor spinner{};
  Rotor bouncer{};
};

class BlossomPanel : public Panel<BlossomPanel> {
public:
  explicit BlossomPanel(Blossom *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = widget_right_edge/7.f;
    auto column_4 = widget_right_edge - column_1;
    auto column_2 = (column_4 - column_1)/3.f + column_1;
    auto column_3 = widget_right_edge - column_2;

    auto y = 25.f;
    auto dy = 18.5f;

    install(column_1, y, input(Blossom::SPIN_CV));
    install(column_2, y, knob<TinyKnob>(Blossom::SPIN_AV));
    install(column_3, y, knob<LargeKnob>(Blossom::SPIN_KNOB, 0.65f));

    y += dy;
    install(column_1, y, input(Blossom::BOUNCE_CV));
    install(column_2, y, knob<TinyKnob>(Blossom::BOUNCE_AV));
    install(column_3, y, knob<LargeKnob>(Blossom::BOUNCE_KNOB));
    install(column_4, y, toggle<2>(Blossom::BOUNCE_LOCK_SWITCH, 1));

    y += dy;
    install(column_1, y, input(Blossom::DEPTH_CV));
    install(column_2, y, knob<TinyKnob>(Blossom::DEPTH_AV));
    install(column_3, y, knob<LargeKnob>(Blossom::DEPTH_KNOB));

    y += dy;
    install(column_1, y, input(Blossom::PHASE_CV));
    install(column_2, y, knob<TinyKnob>(Blossom::PHASE_AV));
    install(column_3, y, knob<LargeKnob>(Blossom::PHASE_KNOB));

    y = 97.f;
    dy = 15.f;
    const auto output_port_offset = 1.25f;

    auto default_gain = Rotation::gain_range.normalize(1.f);

    install(column_1, y, input(Blossom::X_GAIN_CV));
    install(column_2, y, knob<SmallKnob>(Blossom::X_GAIN_KNOB, default_gain));
    install(column_3, y, toggle<2>(Blossom::X_RANGE_SWITCH, 0));
    install(column_4, y + output_port_offset, output(Blossom::X_OUT));

    y += dy;
    install(column_1, y, input(Blossom::Y_GAIN_CV));
    install(column_2, y, knob<SmallKnob>(Blossom::Y_GAIN_KNOB, default_gain));
    install(column_3, y, toggle<2>(Blossom::Y_RANGE_SWITCH, 0));
    install(column_4, y + output_port_offset, output(Blossom::Y_OUT));
  }

  static constexpr auto module_slug = "blossom";

private:
  static constexpr auto hp = 11;
};

} // namespace DHE
rack::Model *modelBlossom =
    rack::Model::create<DHE::Blossom, DHE::BlossomPanel>(
        "DHE-Modules", "Blossom", "Blossom", rack::LFO_TAG);
