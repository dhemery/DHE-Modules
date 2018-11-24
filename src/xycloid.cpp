#include <math.h>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/sigmoid.hpp"
#include "util/signal.hpp"

namespace DHE {

struct XycloidRotor {
  float const two_pi{2.f * std::acos(-1.f)};
  float phase{0.f};
  float offset{0.f};

  void advance(float delta, float offset = 0.f) {
    this->offset = offset;
    phase += delta;
    phase -= std::trunc(phase);
  }

  auto x() const -> float { return std::cos(two_pi * (phase + offset)); }
  auto y() const -> float { return std::sin(two_pi * (phase + offset)); }
};

struct Xycloid : Module {
  enum ParameterIds {
    WOBBLE_RATIO,
    WOBBLE_RATIO_AV,
    WOBBLE_TYPE,
    WOBBLE_DEPTH,
    WOBBLE_DEPTH_AV,
    THROB_SPEED,
    THROB_SPEED_AV,
    X_GAIN,
    Y_GAIN,
    X_RANGE,
    Y_RANGE,
    WOBBLE_RATIO_TYPE,
    WOBBLE_PHASE,
    PARAMETER_COUNT
  };
  enum InputIds {
    WOBBLE_RATIO_CV,
    WOBBLE_DEPTH_CV,
    THROB_SPEED_CV,
    X_GAIN_CV,
    Y_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds { X_OUT, Y_OUT, OUTPUT_COUNT };

  static auto constexpr throb_speed_knob_range = Range{-1.f, 1.f};
  static auto constexpr wobble_depth_range = Range{0.f, 1.f};
  static auto constexpr gain_range = Range{0.f, 2.f};

  float wobble_ratio_offset{0.f};
  XycloidRotor wobbler{};
  XycloidRotor throbber{};

  Xycloid() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto is_wobble_ratio_free() const -> bool {
    return params[WOBBLE_RATIO_TYPE].value > 0.1f;
  }

  void step() override {
    auto wobble_ratio = this->wobble_ratio();
    auto wobble_phase_offset = wobble_phase_in();
    if (wobble_ratio < 0.f)
      wobble_phase_offset *= -1.f;

    auto throb_speed = this->throb_speed();
    auto wobble_speed = wobble_ratio * throb_speed;
    auto wobble_depth = this->wobble_depth();
    auto throb_depth = 1.f - wobble_depth;

    throbber.advance(throb_speed);
    wobbler.advance(wobble_speed, wobble_phase_offset);
    auto x = throb_depth * throbber.x() + wobble_depth * wobbler.x();
    auto y = throb_depth * throbber.y() + wobble_depth * wobbler.y();

    outputs[X_OUT].value = 5.f * x_gain_in() * (x + x_offset());
    outputs[Y_OUT].value = 5.f * y_gain_in() * (y + y_offset());
  }

  auto throb_speed_in() const -> float {
    return modulated(THROB_SPEED, THROB_SPEED_CV, THROB_SPEED_AV);
  }

  auto throb_speed() const -> float {
    auto constexpr speed_taper_curvature = 0.8f;
    auto scaled = throb_speed_knob_range.scale(throb_speed_in());
    auto tapered = Sigmoid::curve(scaled, speed_taper_curvature);
    return -10.f * tapered * rack::engineGetSampleTime();
  }

  auto wobble_depth() const -> float {
    return wobble_depth_range.clamp(wobble_depth_in());
  }

  auto wobble_depth_in() const -> float {
    return modulated(WOBBLE_DEPTH, WOBBLE_DEPTH_CV, WOBBLE_DEPTH_AV);
  }

  auto wobble_phase_in() const -> float {
    auto const rotation{params[WOBBLE_PHASE].value};
    return rotation - 0.5f;
  }

  auto wobble_range() const -> Range const & {
    static auto constexpr wobble_max = 16.f;
    static auto constexpr inward_wobble_range = Range{0.f, wobble_max};
    static auto constexpr outward_wobble_range = Range{0.f, -wobble_max};
    static auto constexpr bidirectional_wobble_range =
        Range{wobble_max, -wobble_max};
    static const std::vector<Range> wobble_ratio_ranges{
        inward_wobble_range, bidirectional_wobble_range, outward_wobble_range};

    return wobble_ratio_ranges[wobble_type()];
  }

  auto wobble_ratio() const -> float {
    auto wobble_ratio =
        wobble_range().scale(wobble_ratio_in()) + wobble_ratio_offset;
    return is_wobble_ratio_free() ? wobble_ratio : std::round(wobble_ratio);
  }

  auto wobble_ratio_in() const -> float {
    return modulated(WOBBLE_RATIO, WOBBLE_RATIO_CV, WOBBLE_RATIO_AV);
  }

  auto wobble_type() const -> int {
    auto const param{params[WOBBLE_TYPE].value};
    return static_cast<int>(param);
  }

  auto x_gain_in() const -> float {
    auto const rotation{modulated(X_GAIN, X_GAIN_CV)};
    return gain_range.scale(rotation);
  }

  auto x_offset() const -> float {
    auto const is_uni{params[X_RANGE].value > 0.1f};
    return is_uni ? 1.f : 0.f;
  }

  auto y_gain_in() const -> float {
    auto const rotation{modulated(Y_GAIN, Y_GAIN_CV)};
    return gain_range.scale(rotation);
  }

  auto y_offset() const -> float {
    auto const is_uni{params[Y_RANGE].value > 0.1f};
    return is_uni ? 1.f : 0.f;
  }

  json_t *toJson() override {
    json_t *configuration = json_object();
    json_object_set_new(configuration, "musical_wobble_ratios",
                        json_boolean(is_musical_wobble_ratios()));
    return configuration;
  }

  void fromJson(json_t *configuration) override {
    json_t *musical_wobble_ratios =
        json_object_get(configuration, "musical_wobble_ratios");
    set_musical_wobble_ratios(json_is_true(musical_wobble_ratios));
  }

  void set_musical_wobble_ratios(bool is_musical) {
    wobble_ratio_offset = is_musical ? 0.f : 1.f;
  }

  bool is_musical_wobble_ratios() { return wobble_ratio_offset == 0.f; }
};

struct XycloidWidget : public ModuleWidget {
  explicit XycloidWidget(rack::Module *module)
      : ModuleWidget(module, 11, "xycloid") {
    auto widget_right_edge = width();

    auto left_x = widget_right_edge / 7.f;
    auto right_x = widget_right_edge - left_x;
    auto left_center_x = (right_x - left_x) / 3.f + left_x;
    auto right_center_x = widget_right_edge - left_center_x;

    auto top_row_y = 30.f;
    auto row_spacing = 22.f;

    auto row = 0;

    install_input(Xycloid::WOBBLE_RATIO_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_knob("tiny", Xycloid::WOBBLE_RATIO_AV,
                 {left_center_x, top_row_y + row * row_spacing});
    install_knob("large", Xycloid::WOBBLE_RATIO,
                 {right_center_x, top_row_y + row * row_spacing});
    install_switch(Xycloid::WOBBLE_RATIO_TYPE,
                   {right_x, top_row_y + row * row_spacing}, 1, 1);

    row++;
    install_input(Xycloid::WOBBLE_DEPTH_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_knob("tiny", Xycloid::WOBBLE_DEPTH_AV,
                 {left_center_x, top_row_y + row * row_spacing});
    install_knob("large", Xycloid::WOBBLE_DEPTH,
                 {right_center_x, top_row_y + row * row_spacing});
    install_switch(Xycloid::WOBBLE_TYPE,
                   {right_x, top_row_y + row * row_spacing}, 2, 2);

    row++;
    install_input(Xycloid::THROB_SPEED_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_knob("tiny", Xycloid::THROB_SPEED_AV,
                 {left_center_x, top_row_y + row * row_spacing});
    install_knob("large", Xycloid::THROB_SPEED,
                 {right_center_x, top_row_y + row * row_spacing}, 0.65f);
    install_knob("small", Xycloid::WOBBLE_PHASE,
                 {right_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;
    row = 0;

    auto default_gain = Xycloid::gain_range.normalize(1.f);
    row++;
    install_input(Xycloid::X_GAIN_CV, {left_x, top_row_y + row * row_spacing});
    install_knob("small", Xycloid::X_GAIN,
                 {left_center_x, top_row_y + row * row_spacing}, default_gain);
    install_switch(Xycloid::X_RANGE,
                   {right_center_x, top_row_y + row * row_spacing});
    install_output(Xycloid::X_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Xycloid::Y_GAIN_CV, {left_x, top_row_y + row * row_spacing});
    install_knob("small", Xycloid::Y_GAIN,
                 {left_center_x, top_row_y + row * row_spacing}, default_gain);
    install_switch(Xycloid::Y_RANGE,
                   {right_center_x, top_row_y + row * row_spacing});
    install_output(Xycloid::Y_OUT, {right_x, top_row_y + row * row_spacing});
  }

  void appendContextMenu(rack::Menu *menu) override {
    auto xycloid = dynamic_cast<Xycloid *>(module);
    assert(xycloid);

    menu->addChild(rack::construct<rack::MenuLabel>());
    menu->addChild(
        rack::construct<rack::MenuLabel>(&rack::MenuLabel::text, "Options"));
    menu->addChild(new BooleanOption(
        "Musical Ratios",
        [xycloid](bool setting) {
          xycloid->set_musical_wobble_ratios(setting);
        },
        [xycloid]() -> bool { return xycloid->is_musical_wobble_ratios(); }));
  }
};

} // namespace DHE
rack::Model *modelXycloid =
    rack::Model::create<DHE::Xycloid, DHE::XycloidWidget>(
        "DHE-Modules", "Xycloid", "Xycloid", rack::FUNCTION_GENERATOR_TAG,
        rack::LFO_TAG);
