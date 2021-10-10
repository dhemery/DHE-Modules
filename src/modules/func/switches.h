#pragma once
#include <string>
#include <vector>

namespace dhe {
namespace func {

struct OffsetRanges {
  using ValueT = int;
  static constexpr auto frame_prefix = "offset-range";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–5 V", "±5 V", "0–10 V", "±10 V"};
    return labels;
  }
};

struct MultiplierRanges {
  using ValueT = int;
  static constexpr auto frame_prefix = "multiplier-range";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–1", "±1", "0–2", "±2"};
    return labels;
  }
};

template <typename PanelT>
class OperatorSwitch : public SwitchWidget<PanelT, ThumbSwitch<2>> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index, std::function<void(bool)> const &action)
      -> OperatorSwitch * {
    auto toggle = rack::createParamCentered<OperatorSwitch>(mm2px(xmm, ymm),
                                                            module, index);
    toggle->operator_changed_to_ = action;
    return toggle;
  }

  OperatorSwitch() = default;

  void onChange(const rack::event::Change &e) override {
    SwitchWidget<PanelT, ThumbSwitch<2>>::onChange(e);
    auto const is_multiply = this->getParamQuantity()->getValue() > 0.5;
    operator_changed_to_(is_multiply);
  }

private:
  std::function<void(bool)> operator_changed_to_ = [](bool /*unused*/) {};
};
} // namespace func

} // namespace dhe
