#pragma once
#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <ui/Label.hpp>
#include <utility>

namespace dhe {
class Menu : public rack::app::ParamWidget {
public:
  static inline auto button(rack::engine::Module *module, float x, float y, int index) -> Menu * {
    return new Menu(module, x, y, index);
  }

private:
  std::vector<rack::ui::Label *> labels{};
  unsigned int selection{};

  auto makeLabel(std::string text) -> rack::ui::Label * {
    auto *label = new rack::ui::Label;
    label->text = std::move(text);
    label->color = nvgRGBf(1.F, 0.F, 0.F);
    addChild(label);
    return label;
  }

  Menu(rack::engine::Module *module, float x, float y, int index) {
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }

    setSize(rack::math::Vec{hp2px(1.F), hp2px(1.F)});
    positionCentered(this, x, y);
    labels.push_back(makeLabel("foo"));
    labels.push_back(makeLabel("bar"));
    labels.push_back(makeLabel("baz"));
  }

public:
  void onAction(rack::event::Action const &action) override {
    selection++;
    if (selection >= labels.size()) {
      selection = 0;
    }
    paramQuantity->setValue(static_cast<float>(selection));
    action.consume(this);
  }

  void onButton(rack::event::Button const &buttonEvent) override {
    static auto const buttonPressed = rack::event::Action{};

    if (buttonEvent.action == GLFW_PRESS && buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
      buttonEvent.consume(this);
      onAction(buttonPressed);
    }
  }

private:
  void draw(const DrawArgs &args) override { labels[selection]->draw(args); }
};

} // namespace dhe
