#pragma once

class SVGLoader {
public:
  explicit SVGLoader(const std::string &module_dir) : resource_dir{resource_dir_for(module_dir)} {}

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

class Potentiometer : public rack::RoundKnob, SVGLoader {
public:
  Potentiometer(const std::string &module_dir, const std::string &size) : SVGLoader{module_dir} {
    static const auto prefix = std::string{"knob-"};
    setSVG(load_svg(prefix + size));
    shadow->opacity = 0.f;
  }
};

class LargeKnob : public Potentiometer {
public:
  explicit LargeKnob(const std::string &module_dir) : Potentiometer(module_dir, "large") {}
};

class MediumKnob : public Potentiometer {
public:
  explicit MediumKnob(const std::string &module_dir) : Potentiometer(module_dir, "medium") {}
};

class SmallKnob : public Potentiometer {
public:
  explicit SmallKnob(const std::string &module_dir) : Potentiometer(module_dir, "small") {}
};

class TinyKnob : public Potentiometer {
public:
  explicit TinyKnob(const std::string &module_dir) : Potentiometer(module_dir, "tiny") {}
};


class Button : public rack::SVGSwitch, public rack::MomentarySwitch, public SVGLoader {
public:
  Button(const std::string &module_dir, const std::string &type) : SVGLoader{module_dir} {
    static const auto base = std::string{"button-"};
    const auto prefix = base + type + "-";
    addFrame(load_svg(prefix + "1"));
    addFrame(load_svg(prefix + "2"));
  }

  explicit Button(const std::string &module_dir) : Button(module_dir, "normal") {}
};
