#pragma once
#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <ui/Label.hpp>
#include <utility>

namespace dhe {
class Menu : public rack::app::ParamWidget {
public:
  static inline auto button(rack::engine::Module *module, const std::vector<std::string> &labels, float x, float y,
                            int index) -> Menu * {
    return new Menu(module, labels, x, y, index);
  }

  void onAction(rack::event::Action const &action) override {
    selection++;
    if (selection >= labelWidgets.size()) {
      selection = 0;
    }
    paramQuantity->setValue(static_cast<float>(selection));
    action.consume(this);
  }

  void onButton(rack::event::Button const &buttonEvent) override {
    static auto const buttonPressed = rack::event::Action{};

    if (buttonEvent.action == GLFW_PRESS && buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
      onAction(buttonPressed);
      buttonEvent.consume(this);
    }
  }

  void draw(const DrawArgs &args) override {
    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 5.0);
    auto const shade = 0.8F;
    nvgFillColor(args.vg, nvgRGBf(shade, shade, shade));
    nvgFill(args.vg);

    rack::app::ParamWidget::draw(args);
    labelWidgets[selection]->draw(args);
  }

private:
  std::vector<rack::ui::Label *> labelWidgets{};
  unsigned int selection{};

  static auto makeLabel(std::string text) -> rack::ui::Label * {
    auto *label = new rack::ui::Label;
    label->text = std::move(text);
    label->color = nvgRGBf(1.F, 0.F, 0.F);
    return label;
  }

  Menu(rack::engine::Module *module, const std::vector<std::string> &labels, float x, float y, int index) {
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }

    setSize(rack::math::Vec{hp2px(2.F), hp2px(1.F)});
    positionCentered(this, x, y);
    for (auto const &label : labels) {
      labelWidgets.push_back(makeLabel(label));
    }
  }
};

} // namespace dhe
