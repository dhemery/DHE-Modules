#pragma once
#include "Dimensions.h"

#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <helpers.hpp>
#include <ui/Label.hpp>
#include <utility>

namespace dhe {
namespace picklist {

  class Option : public rack::ui::MenuItem {
  public:
    Option(float value, const std::string &text, std::function<void(float, std::string const &)> onSelection) :
        value{value}, onSelection{std::move(onSelection)} {
      this->text = text;
    }

    void onAction(const rack::event::Action &action) override {
      onSelection(value, text);
      rack::ui::MenuItem::onAction(action);
    }

  private:
    float value;
    std::function<void(float, std::string const &)> onSelection;
  };

  class Button : public rack::app::ParamWidget {
  public:
    Button(rack::engine::Module *module, const std::vector<std::string> &optionNames, float xmm, float ymm, int index) {
      if (module != nullptr) {
        paramQuantity = module->paramQuantities[index];
      }

      setSize(rack::math::Vec{hp2px(2.F), hp2px(1.F)});
      positionCentered(this, xmm, ymm);

      auto updateSelection = [this](float value, std::string const &label) { select(value, label); };

      for (size_t i = 0; i < optionNames.size(); i++) {
        auto optionValue = static_cast<float>(i);
        auto optionName = optionNames[i];
        auto *option = new Option(optionValue, optionName, updateSelection);
        popupMenu->addChild(option);
      }

      popupMenu->hide();
      positionCentered(popupMenu, xmm, ymm);

      selection->text = optionNames[0];
      selection->color = nvgRGBf(1.F, 0.F, 0.F);
      addChild(selection);
    }

    void onAction(rack::event::Action const &action) override {
      if (popupMenu->visible) {
        popupMenu->hide();
      } else {
        popupMenu->show();
      }
      action.consume(this);
    }

    void onButton(rack::event::Button const &buttonEvent) override {
      static auto const buttonPressed = rack::event::Action{};

      if (buttonEvent.action == GLFW_PRESS && buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
        onAction(buttonPressed);
        buttonEvent.consume(this);
      }
    }

    auto menu() -> rack::ui::Menu * { return popupMenu; }

  private:
    void select(float value, const std::string &label) const {
      selection->text = label;
      paramQuantity->setValue(value);
      popupMenu->hide();
    }

    rack::ui::Menu *const popupMenu{new rack::ui::Menu};
    rack::ui::Label *const selection{new rack::ui::Label};
  };

  static inline auto button(rack::engine::Module *module, const std::vector<std::string> &labels, float xmm, float ymm,
                            int index) -> Button * {
    return new Button(module, labels, xmm, ymm, index);
  }
} // namespace picklist

} // namespace dhe
