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

    def draw_manual(svg:, x:, y:)
      draw_faceplate(svg: svg, x: x, y: y)
      draw_hardware(svg: svg, x: x, y: y)
    end

    def to_svg
      Builder::XmlMarkup.new(indent: 2)
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

    def draw_faceplate(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @button.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
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

    def draw_faceplate(svg:, x:, y:)
    end

    def draw_hardware(svg:, x:, y:)
      @labels[@selection - 1].draw_svg(svg: svg, x: x, y: @button.top(y - PADDING)) unless @invisible
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

    def draw_faceplate(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @knob.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @knob.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class PortControl < Control
    def initialize(panel, options)
      super(options)
      foreground = panel.foreground
      background = panel.background
      style = options[:style]
      label_text = options[:label] || style
      is_input = style == 'in'
      is_output = style == 'out'
      label_color = is_output ? background : foreground
      @label = Label.new(text: label_text, size: :small, color: label_color, alignment: :above)
      @port = Port.new(foreground: foreground, background: background)
      has_button = options[:button]
      button_style = is_input ? :normal : :reversed
      box_left = @port.left(0)
      box_right = @port.right(0)
      box_top = @label.top(@port.top(0))
      box_bottom = @port.bottom(0)
      @button_offset = PADDING + Button::DIAMETER / 2 + @port.width / 2
      if has_button
        if is_input
          box_right = @port.right(0) + PADDING + Button::DIAMETER
        else
          box_left = @port.left(0) - PADDING - Button::DIAMETER
          @button_offset *= -1
        end
        @button = Button.new(foreground: foreground, background: background, style: button_style)
      end
      is_boxed = is_input || is_output
      box_background = is_output ? foreground : background
      @box = Box.new(top: box_top, right: box_right, bottom: box_bottom, left: box_left, foreground: foreground, background: box_background) if is_boxed
    end

    def draw_faceplate(x:, y:, svg:)
      @box.draw_svg(svg: svg, x: x, y: y) if @box
      @label.draw_svg(svg: svg, x: x, y: @port.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @port.draw_svg(svg: svg, x: x, y: y)
      @button.draw_svg(svg: svg, x: x + @button_offset, y: y) if @button
    end

    def svg_file(module_path:)
      SvgFile.new(path: module_path / path, content: to_svg)
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

    def draw_faceplate(svg:, x:, y:)
      @labels.first.draw_svg(svg: svg, x: x, y: @toggle.bottom(y + PADDING))
      @labels[1].draw_svg(svg: svg, x: @toggle.right(x + PADDING / 2), y: y) if (@labels.size == 3)
      @labels.last.draw_svg(svg: svg, x: x, y: @toggle.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @toggle.draw_svg(svg: svg, x: x, y: y, selection: @selection)
    end
  end
end
