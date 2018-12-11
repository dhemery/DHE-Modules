#pragma once

namespace DHE {
class Control {
public:
  explicit Control(const std::string &module_dir)
      : resource_dir{resource_dir_for(module_dir)} {}

protected:
  auto load_svg(const std::string &filename) -> std::shared_ptr<rack::SVG> {
    return rack::SVG::load(resource_dir + filename + ".svg");
  }

private:
  static auto resource_dir_for(const std::string &module_dir) -> std::string {
    static const auto plugin_resource_root = rack::assetPlugin(plugin, "res/");
    return plugin_resource_root + module_dir + "/";
  }

  const std::string resource_dir;
};

class Potentiometer : public rack::RoundKnob, public Control {
public:
  Potentiometer(const std::string &module_dir, const std::string &size)
      : Control{module_dir} {
    static const auto prefix = std::string{"knob-"};
    setSVG(load_svg(prefix + size));
    shadow->opacity = 0.f;
  }
};

class LargeKnob : public Potentiometer {
public:
  explicit LargeKnob(const std::string &module_dir)
      : Potentiometer(module_dir, "large") {}
};

class MediumKnob : public Potentiometer {
public:
  explicit MediumKnob(const std::string &module_dir)
      : Potentiometer(module_dir, "medium") {}
};

class SmallKnob : public Potentiometer {
public:
  explicit SmallKnob(const std::string &module_dir)
      : Potentiometer(module_dir, "small") {}
};

class TinyKnob : public Potentiometer {
public:
  explicit TinyKnob(const std::string &module_dir)
      : Potentiometer(module_dir, "tiny") {}
};

class Button : public rack::SVGSwitch,
               public rack::MomentarySwitch,
               public Control {
public:
  Button(const std::string &module_dir, const std::string &type)
      : Control{module_dir} {
    static const auto base = std::string{"button-"};
    const auto prefix = base + type + "-";
    addFrame(load_svg(prefix + "1"));
    addFrame(load_svg(prefix + "2"));
  }

  explicit Button(const std::string &module_dir)
      : Button(module_dir, "normal") {}
};

class ThumbSwitch : public rack::SVGSwitch,
                    public rack::ToggleSwitch,
                    public Control {
public:
  ThumbSwitch(const std::string &module_dir, int size) : Control{module_dir} {
    static const auto base = std::string{"thumb-"};
    const auto prefix = base + std::to_string(size) + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(load_svg(prefix + std::to_string(position)));
    }
  }
};

class ThumbSwitch2 : public ThumbSwitch {
public:
  explicit ThumbSwitch2(const std::string &module_dir)
      : ThumbSwitch{module_dir, size} {}

  static constexpr auto size = 2;
};

class ThumbSwitch3 : public ThumbSwitch {
public:
  explicit ThumbSwitch3(const std::string &module_dir)
      : ThumbSwitch{module_dir, size} {}

  static constexpr auto size = 3;
};
} // namespace DHE
