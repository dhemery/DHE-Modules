require_relative 'dimensions'
require_relative 'shape'

module DHE
  class Control
    attr_reader :name, :row, :column

    def initialize(options)
      @name = options[:name]
      @row = options[:row]
      @column = options[:column]
    end

    def draw_image_svg(svg:, x:, y:)
      draw_panel_svg(svg: svg, x: x, y: y)
      draw_hardware_svg(svg: svg, x: x, y: y)
    end
  end

  class ButtonControl < Control
    DIAMETER = 6.0

    def initialize(panel, options)
      super(options)
      foreground = panel.foreground
      background = panel.background
      label_text = options[:label]
      @label = Label.new(text: label_text, size: :small, alignment: :above, color: foreground)
      @button = Button.new(foreground: foreground, background: background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @button.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @button.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class CounterControl < Control
    def initialize(panel, options)
      super(options)
      foreground = panel.foreground
      background = panel.background
      label_texts = options[:labels]
      @invisible = options.fetch(:invisible, false)
      @selection = options.fetch(:selection, 1)
      @labels = label_texts.map {|text| Label.new(text: text, color: foreground, size: :small, alignment: :above)}
      @button = Button.new(foreground: foreground, background: background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @labels[@selection - 1].draw_svg(svg: svg, x: x, y: @button.top(y: y)) unless @invisible
    end

    def draw_hardware_svg(svg:, x:, y:)
      @button.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class KnobControl < Control
    def initialize(panel, options)
      super(options)
      foreground = panel.foreground
      background = panel.background
      label_text = options[:label]
      @style = options.fetch(:style, :large)
      @label = Label.new(text: label_text, size: :large, color: foreground, alignment: :above)
      @knob = Knob.new(size: @style.to_sym, knob_color: foreground, pointer_color: background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @knob.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @knob.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class PortControl < Control
    def initialize(panel, options)
      super(options)
      foreground = panel.foreground
      background = panel.background
      label_text = options[:label] || options[:style]
      @label = Label.new(text: label_text, size: :small, color: foreground, alignment: :above)
      @port = Port.new(foreground: foreground, background: background)
    end

    def draw_panel_svg(x:, y:, svg:)
      @label.draw_svg(svg: svg, x: x, y: @port.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @port.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class ToggleControl < Control
    def initialize(panel, options)
      super(options)
      foreground = panel.foreground
      background = panel.background
      label_texts = options[:labels]
      @selection = options.fetch(:selection, 1)
      @labels = Array(Label.new(text: label_texts.first, size: :small, color: foreground, alignment: :below))
      @labels << Label.new(text: label_texts[1], size: :small, color: foreground, alignment: :right_of) if (label_texts.size == 3)
      @labels << Label.new(text: label_texts.last, size: :small, color: foreground, alignment: :above)
      @toggle = Toggle.new(size: label_texts.size, foreground: foreground, background: background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @labels.first.draw_svg(svg: svg, x: x, y: @toggle.bottom(y: y))
      @labels[1].draw_svg(svg: svg, x: @toggle.right(x: x), y: y) if (@labels.size == 3)
      @labels.last.draw_svg(svg: svg, x: x, y: @toggle.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @toggle.draw_svg(svg: svg, x: x, y: y, selection: @selection)
    end
  end
end
