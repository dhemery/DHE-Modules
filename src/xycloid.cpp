#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/sigmoid.hpp"

namespace DHE {

struct Pole {
  const float two_pi{2.f * std::acos(-1.f)};
  float phase{0.f};
  float offset{0.f};

  void advance(float increment, float offset = 0.f) {
    this->offset = offset;
    phase += increment;
    phase -= std::trunc(phase);
  }

  auto x() const -> float { return std::cos(two_pi * (phase + offset)); }
  auto y() const -> float { return std::sin(two_pi * (phase + offset)); }
};

struct Xycloid : Module {
  static constexpr auto throb_speed_knob_range = Range{-1.f, 1.f};
  static constexpr auto wobble_depth_range = Range{0.f, 1.f};
  static constexpr auto gain_range = Range{0.f, 2.f};

  std::function<float()> const x_offset{knob(X_RANGE)};
  std::function<float()> const y_offset{knob(Y_RANGE)};
  std::function<float()> const wobble_phase_knob{knob(WOBBLE_PHASE)};
  std::function<float()> const wobble_ratio_knob{
      knob(WOBBLE_RATIO, WOBBLE_RATIO_CV, WOBBLE_RATIO_AV)};
  std::function<float()> const wobble_depth_knob{
      knob(WOBBLE_DEPTH, WOBBLE_DEPTH_CV, WOBBLE_DEPTH_AV)};
  std::function<bool()> const is_wobble_ratio_free{
      bool_param(WOBBLE_RATIO_TYPE)};
  std::function<int()> const wobble_type{int_param(WOBBLE_TYPE)};
  std::function<float()> const throb_speed_knob{
      knob(THROB_SPEED, THROB_SPEED_CV, THROB_SPEED_AV)};
  std::function<float()> const x_gain_knob{knob(X_GAIN, X_GAIN_CV)};
  std::function<float()> const y_gain_knob{knob(Y_GAIN, Y_GAIN_CV)};

  float wobble_ratio_offset{0.f};
  Pole wobbler{};
  Pole throbber{};

  Xycloid() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto wobble_range() const -> Range const & {
    static constexpr auto wobble_max = 16.f;
    static constexpr auto inward_wobble_range = Range{0.f, wobble_max};
    static constexpr auto outward_wobble_range = Range{0.f, -wobble_max};
    static constexpr auto bidirectional_wobble_range =
        Range{wobble_max, -wobble_max};
    static const std::vector<Range> wobble_ratio_ranges{
        inward_wobble_range, bidirectional_wobble_range, outward_wobble_range};

    return wobble_ratio_ranges[wobble_type()];
  }

  auto wobble_ratio() const -> float {
    auto wobble_ratio =
        wobble_range().scale(wobble_ratio_knob()) + wobble_ratio_offset;
    return is_wobble_ratio_free() ? wobble_ratio : std::round(wobble_ratio);
  }

  auto wobble_depth() const -> float {
    return wobble_depth_range.clamp(wobble_depth_knob());
  }

  auto throb_speed() const -> float {
    auto scaled = throb_speed_knob_range.scale(throb_speed_knob());
    auto tapered = sigmoid(scaled, 0.8f);
    return -10.f * tapered * rack::engineGetSampleTime();
  }

  void step() override {
    auto wobble_ratio = this->wobble_ratio();
    auto wobble_phase_offset = wobble_phase_knob() - 0.5f;
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

    auto x_gain = gain_range.scale(x_gain_knob());
    auto y_gain = gain_range.scale(y_gain_knob());
    outputs[X_OUT].value = 5.f * x_gain * (x + x_offset());
    outputs[Y_OUT].value = 5.f * y_gain * (y + y_offset());
  }

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
