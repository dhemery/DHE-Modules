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
    Option(int index, const std::string &label, std::function<void(int)> onSelection) :
        index{index}, onSelection{std::move(onSelection)} {
      this->text = label;
    }

    void onAction(rack::event::Action const &action) override {
      MenuItem::onAction(action);
      onSelection(index);
      action.consume(this);
    }

  private:
    int index;
    std::function<void(int)> onSelection;
  };

  class Button : public rack::app::ParamWidget {
  public:
    Button(rack::engine::Module *module, const std::vector<std::string> &labels, float xmm, float ymm, int index) :
        labels{labels} {
      if (module != nullptr) {
        paramQuantity = module->paramQuantities[index];
      }

      setSize(rack::math::Vec{hp2px(2.F), hp2px(1.F)});
      positionCentered(this, xmm, ymm);

      auto updateSelection = [this](int index) { select(index); };

      for (size_t i = 0; i < labels.size(); i++) {
        auto *option = new Option(i, labels[i], updateSelection);
        popupMenu->addChild(option);
      }

      popupMenu->hide();
      positionCentered(popupMenu, xmm, ymm);

      label->text = labels[0];
      label->color = nvgRGBf(1.F, 0.F, 0.F);
      addChild(label);
    }

    void onAction(rack::event::Action const &action) override {
      ParamWidget::onAction(action);
      if (popupMenu->visible) {
        popupMenu->hide();
      } else {
        popupMenu->show();
      }
      action.consume(this);
    }

    void onButton(rack::event::Button const &buttonEvent) override {
      ParamWidget::onButton(buttonEvent);
      static auto const buttonPressed = rack::event::Action{};

      if (buttonEvent.action == GLFW_PRESS && buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
        onAction(buttonPressed);
        buttonEvent.consume(this);
      }
    }

    void onChange(rack::event::Change const &changeEvent) override {
      ParamWidget::onChange(changeEvent);
      auto selection = static_cast<int>(paramQuantity->getValue());
      label->text = labels[selection];
      changeEvent.consume(this);
    }

    auto menu() -> rack::ui::Menu * { return popupMenu; }

  private:
    void select(int selection) const {
      paramQuantity->setValue(static_cast<float>(selection));
      popupMenu->hide();
    }

    rack::ui::Menu *const popupMenu{new rack::ui::Menu};
    rack::ui::Label *const label{new rack::ui::Label};
    std::vector<std::string> const labels;
  };

  static inline auto button(rack::engine::Module *module, const std::vector<std::string> &labels, float xmm, float ymm,
                            int index) -> Button * {
    return new Button(module, labels, xmm, ymm, index);
  }
} // namespace picklist

} // namespace dhe
