#pragma once
#include "Dimensions.h"

#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <helpers.hpp>
#include <ui/Label.hpp>
#include <utility>
#include <widget/SvgWidget.hpp>

namespace dhe {
namespace picklist {
  class Option : public rack::ui::MenuItem {
  public:
    Option(std::string const &label, std::function<void()> onPick) : onPick{std::move(onPick)} { this->text = label; }

    void onAction(rack::event::Action const &action) override {
      MenuItem::onAction(action);
      onPick();
      action.consume(this);
    }

  private:
    std::function<void()> onPick;
  };

  class Menu : public rack::ui::Menu {
  public:
    void hideIfInactive() {
      if (!isActive) {
        hide();
      }
    }

    void onEnter(const rack::event::Enter &entryEvent) override {
      Widget::onEnter(entryEvent);
      isActive = true;
      entryEvent.consume(this);
    }

    void onHover(const rack::event::Hover &hoverEvent) override {
      OpaqueWidget::onHover(hoverEvent);
      hoverEvent.consume(this); // registers for Enter and Leave events
    }

    void onLeave(const rack::event::Leave &exitEvent) override {
      Widget::onLeave(exitEvent);
      isActive = false;
      exitEvent.consume(this);
    }

  private:
    bool isActive{};
  };

  class Label : public rack::widget::SvgWidget {
  public:
    Label(std::string const &listName, int index, std::string const &moduleSvgDir) {
      static auto const basename = std::string{"picklist-"};
      auto const filename = basename + listName + '-' + std::to_string(index + 1);
      setSvg(controlSvg(moduleSvgDir, filename));
    }
  };

  class Button : public rack::app::ParamWidget {
  public:
    Button(std::string const &listName, std::vector<std::string> const &optionNames, std::string const &moduleSvgDir,
           rack::engine::Module *module, float xmm, float ymm, int index) {
      if (module != nullptr) {
        paramQuantity = module->paramQuantities[index];
      }

      setSize(rack::math::Vec{hp2px(2.F), hp2px(1.F)});
      positionCentered(this, xmm, ymm);

      for (size_t i = 0; i < optionNames.size(); i++) {
        auto const onPick = [this, i]() {
          auto const value = static_cast<float>(i);
          paramQuantity->setValue(value);
        };
        auto *option = new Option(optionNames[i], onPick);
        popupMenu->addChild(option);
      }

      popupMenu->hide();
      positionCentered(popupMenu, xmm, ymm);

      for (size_t i = 0; i < optionNames.size(); i++) {
        auto *optionLabel = new Label(listName, i, moduleSvgDir);
        optionLabel->hide();
        optionLabels.push_back(optionLabel);
        addChild(optionLabel);
      }
      selectedOptionLabel = optionLabels[0];
      selectedOptionLabel->show();
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
        buttonEvent.consume(this); // registers for Deselect events
      }
    }

    void onChange(rack::event::Change const &changeEvent) override {
      ParamWidget::onChange(changeEvent);
      auto const selection = static_cast<int>(paramQuantity->getValue());
      selectedOptionLabel->hide();
      selectedOptionLabel = optionLabels[selection];
      selectedOptionLabel->show();
      popupMenu->hide();
      changeEvent.consume(this);
    }

    void onDeselect(const rack::event::Deselect &deselectionEvent) override {
      Widget::onDeselect(deselectionEvent);
      popupMenu->hideIfInactive();
      deselectionEvent.consume(this);
    }

    auto menu() -> rack::ui::Menu * { return popupMenu; }

  private:
    Menu *const popupMenu{new Menu};
    std::vector<Label *> optionLabels;
    Label *selectedOptionLabel;
  };

  static inline auto button(std::string const &listName, std::vector<std::string> const &optionLabels,
                            std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Button * {
    return new Button(listName, optionLabels, moduleSvgDir, module, xmm, ymm, index);
  }
} // namespace picklist

} // namespace dhe
