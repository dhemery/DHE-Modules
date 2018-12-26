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

    def control_files(module_path:)
      [:on, :off].map {|state| @button.svg_file(module_path: module_path, state: state)}
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

    def control_file(module_path:, label:, position:)
      path = module_path / "counter-#{@name}-#{position}"
      width = @button.width
      height = @button.height + (PADDING + label.height) * 2
      x = width / 2
      y = height / 2
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        @button.draw_svg(svg: svg, x: x, y: y)
        label.draw_svg(svg: svg, x: x, y: @button.top(y - PADDING))
      end
      SvgFile.new(path: path, content: content)
    end

    def control_files(module_path:)
      @labels.each_with_index.map {|label, i| control_file(module_path: module_path, label: label, position: i + 1)}
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

    def control_files(module_path:)
      [@knob.svg_file(module_path: module_path)]
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
      is_boxed = is_input || is_output
      label_color = is_output ? background : foreground
      @label = Label.new(text: label_text, size: :small, color: label_color, alignment: :above)
      @port = Port.new(foreground: foreground, background: background)
      box_top = @label.top(@port.top(0))
      box_bottom = @port.bottom(0)
      box_left = @port.left(0)
      box_right = @port.right(0)
      button_position = options.fetch(:button, :none).to_sym
      if button_position != :none
        button_style = is_input ? :normal : :reversed
        @button = Button.new(foreground: foreground, background: background, style: button_style)
        if button_position == :right
          @button_offset = @port.right(0) + PADDING + @button.radius
          box_right = @button.right(@button_offset)
        else
          @button_offset = @port.left(0) - PADDING - @button.radius
          box_left = @button.left(@button_offset)
        end
      end
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

    def control_files(module_path:)
      files = [@port.svg_file(module_path: module_path)]
      files += [:on, :off].map {|state| @button.svg_file(module_path: module_path, state: state)} if @button
      files
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

    def control_files(module_path:)
      (1..@toggle.size).map {|selection| @toggle.svg_file(module_path: module_path, selection: selection)}
    end
  end
end
