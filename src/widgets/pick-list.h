#pragma once
#include "dimensions.h"

#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <functional>
#include <helpers.hpp>
#include <random.hpp>
#include <utility>
#include <widget/FramebufferWidget.hpp>
#include <widget/SvgWidget.hpp>
#include <window.hpp>

namespace dhe {
namespace picklist {

class Item : public rack::widget::OpaqueWidget {
public:
  Item(std::string const &svg_dir, std::string const &menu_name, int index,
       std::function<void(int)> on_pick)
      : index_{index}, on_pick_{std::move(on_pick)} {
    auto const control_name = menu_name + '-' + std::to_string(index + 1);
    auto *label = new rack::widget::SvgWidget{};
    label->setSvg(control_svg(svg_dir, control_name));
    addChild(label);
    setSize(label->box.size);
  }

  /**
   * Pass this item's index to <tt>onPick(int)</tt> and hide the parent menu.
   */
  void onAction(rack::event::Action const &action) override {
    OpaqueWidget::onAction(action);
    on_pick_(index_);
    parent->hide();
    action.consume(this);
  }

  /**
   * Take action if the button event is a left mouse button press.
   */
  void onButton(rack::event::Button const &button_event) override {
    OpaqueWidget::onButton(button_event);
    if (button_event.button == GLFW_MOUSE_BUTTON_LEFT) {
      if (button_event.action == GLFW_PRESS) {
        onAction(rack::event::Action{});
      }
      button_event.consume(this);
    }
  }

private:
  int const index_;
  std::function<void(int)> on_pick_;
};

class Menu : public rack::widget::OpaqueWidget {
public:
  Menu(std::string const &svg_dir, std::string const &menu_name, int size,
       std::function<void(int)> const &on_pick) {
    auto const control_name = menu_name + "-menu";
    auto *background = new rack::widget::SvgWidget{};
    background->setSvg(control_svg(svg_dir, control_name));
    addChild(background);
    setSize(background->box.size);
    auto const inset = mm2px(0.5F);
    auto item_top = inset;
    for (auto option_index = 0; option_index < size; option_index++) {
      auto *item = new Item{svg_dir, menu_name, option_index, on_pick};
      item->setPosition({inset, item_top});
      addChild(item);
      item_top += item->box.size.y;
    }
    hide();
  }

  /**
   * Hide unless the mouse is hovering over this menu.
   */
  void hide_if_inactive() {
    if (!is_hovered_) {
      hide();
    }
  }

  /**
   * Note that the mouse is now hovering over this menu.
   */
  void onEnter(const rack::event::Enter &entry_event) override {
    OpaqueWidget::onEnter(entry_event);
    is_hovered_ = true;
    entry_event.consume(this);
  }

  /**
   * Consume the hover events so that Rack will report Enter and Leave events.
   */
  void onHover(const rack::event::Hover &hover_event) override {
    OpaqueWidget::onHover(hover_event);
    hover_event.consume(this); // registers for Enter and Leave events
  }

  /**
   * Note that the mouse is no longer hovering over this menu.
   */
  void onLeave(const rack::event::Leave &exit_event) override {
    OpaqueWidget::onLeave(exit_event);
    is_hovered_ = false;
    exit_event.consume(this);
  }

private:
  bool is_hovered_{};
};

class Button : public rack::app::ParamWidget {
public:
  Button(std::string const &svg_dir, std::string const &menu_name, int size,
         rack::engine::Module *module, float xmm, float ymm, int index) {
    frame_buffer_ = new rack::widget::FramebufferWidget{};
    addChild(frame_buffer_);

    label_ = new rack::widget::SvgWidget{};
    frame_buffer_->addChild(label_);

    for (int option_index = 0; option_index < size; option_index++) {
      auto const control_name =
          menu_name + '-' + std::to_string(option_index + 1);
      add_frame(control_svg(svg_dir, control_name));
    }

    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }

    position_centered(this, xmm, ymm);

    auto const on_pick = [this](int option_index) {
      auto const value = static_cast<float>(option_index);
      paramQuantity->setValue(value);
    };

    popup_menu_ = new Menu{svg_dir, menu_name, size, on_pick};
    popup_menu_->setPosition({box.pos.x, box.pos.y + box.size.y});
  }

  /**
   * Toggle the menu visibility.
   */
  void onAction(rack::event::Action const &toggle_menu_visibility) override {
    ParamWidget::onAction(toggle_menu_visibility);
    if (popup_menu_->visible) {
      popup_menu_->hide();
    } else {
      popup_menu_->show();
    }
    toggle_menu_visibility.consume(this);
  }

  /**
   * Take action if the button event is a left mouse button press.
   */
  void onButton(rack::event::Button const &button_event) override {
    ParamWidget::onButton(button_event);

    button_event.stopPropagating();
    if (button_event.button == GLFW_MOUSE_BUTTON_LEFT) {
      if (button_event.action == GLFW_PRESS) {
        onAction(rack::event::Action{});
      }
      button_event.consume(this);
    }
  }

  /**
   * Display the newly selected option.
   */
  void onChange(rack::event::Change const &change_event) override {
    auto const selection = static_cast<int>(paramQuantity->getValue());
    label_->setSvg(label_svgs_[selection]);
    frame_buffer_->dirty = true;
    ParamWidget::onChange(change_event);
    change_event.consume(this);
  }

  /**
   * Dismiss the menu unless it now selected.
   */
  void onDeselect(const rack::event::Deselect &deselection_event) override {
    ParamWidget::onDeselect(deselection_event);
    popup_menu_->hide_if_inactive();
    deselection_event.consume(this);
  }

  /**
   * Dismiss the menu.
   */
  void onHide(const rack::event::Hide &hide_event) override {
    ParamWidget::onHide(hide_event);
    popup_menu_->hide();
  }

  void reset() override { paramQuantity->reset(); }

  void randomize() override {
    float value =
        paramQuantity->getMinValue() +
        std::floor(rack::random::uniform() * (paramQuantity->getRange() + 1));
    paramQuantity->setValue(value);
  }

  auto menu() -> rack::widget::Widget * { return popup_menu_; }

private:
  void add_frame(std::shared_ptr<rack::Svg> const &svg) {
    label_svgs_.push_back(svg);
    if (!label_->svg) {
      label_->setSvg(svg);
      setSize(label_->box.size);
      frame_buffer_->setSize(label_->box.size);
    }
  }

  std::vector<std::shared_ptr<rack::Svg>> label_svgs_{};
  rack::widget::FramebufferWidget *frame_buffer_{};
  rack::widget::SvgWidget *label_{};
  Menu *popup_menu_;
};

static inline auto button(std::string const &svg_dir, std::string const &name,
                          int size, rack::engine::Module *module, float xmm,
                          float ymm, int index) -> Button * {
  return new Button{svg_dir, name, size, module, xmm, ymm, index};
}
} // namespace picklist

} // namespace dhe
