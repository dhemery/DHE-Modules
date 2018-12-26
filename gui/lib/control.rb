require_relative 'dimensions'
require_relative 'shape'

module DHE
  class Control
    attr_reader :name, :row, :column

    def initialize(module_, options)
      @module = module_
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

    def initialize(module_, options)
      super(module_, options)
      label_text = options[:label]
      @label = Label.new(module_: module_, text: label_text, size: :small)
      @button = Button.new(module_: module_)
    end

    def draw_faceplate(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @button.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @button.draw_svg(svg: svg, x: x, y: y)
    end

    def control_files
      [:on, :off].map {|state| @button.svg_file(state: state)}
    end
  end

  class CounterControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_texts = options[:labels]
      @invisible = options.fetch(:invisible, false)
      @selection = options.fetch(:selection, 1)
      @labels = label_texts.map {|text| Label.new(module_: module_, text: text, size: :small)}
      @button = Button.new(module_: module_)
    end

    def draw_faceplate(svg:, x:, y:)
    end

    def draw_hardware(svg:, x:, y:)
      @labels[@selection - 1].draw_svg(svg: svg, x: x, y: @button.top(y - PADDING)) unless @invisible
      @button.draw_svg(svg: svg, x: x, y: y)
    end

    def control_file(label:, position:)
      path = @module.slug / "counter-#{@name}-#{position}"
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

    def control_files
      @labels.each_with_index.map {|label, i| control_file(label: label, position: i + 1)}
    end
  end

  class KnobControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_text = options[:label]
      @style = options.fetch(:style, :large)
      @label = Label.new(module_: module_, text: label_text, size: :large)
      @knob = Knob.new(module_: module_, size: @style.to_sym)
    end

    def draw_faceplate(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @knob.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @knob.draw_svg(svg: svg, x: x, y: y)
    end

    def control_files
      [@knob.svg_file]
    end
  end

  class PortControl < Control
    def initialize(module_, options)
      super(module_, options)
      style = options[:style]
      label_text = options[:label] || style
      is_input = style == 'in'
      is_output = style == 'out'
      is_boxed = is_input || is_output
      drawing_style = is_output ? :reversed : :normal
      @label = Label.new(module_: module_, text: label_text, size: :small, style: drawing_style)
      @port = Port.new(module_: module_)
      box_top = @label.top(@port.top(0))
      box_bottom = @port.bottom(0)
      box_left = @port.left(0)
      box_right = @port.right(0)
      button_position = options.fetch(:button, :none).to_sym
      if button_position != :none
        @button = Button.new(module_: module_, style: drawing_style)
        if button_position == :right
          @button_offset = @port.right(0) + PADDING + @button.radius
          box_right = @button.right(@button_offset)
        else
          @button_offset = @port.left(0) - PADDING - @button.radius
          box_left = @button.left(@button_offset)
        end
      end
      @box = Box.new(module_: module_, top: box_top, right: box_right, bottom: box_bottom, left: box_left, style: drawing_style) if is_boxed
    end

    def draw_faceplate(x:, y:, svg:)
      @box.draw_svg(svg: svg, x: x, y: y) if @box
      @label.draw_svg(svg: svg, x: x, y: @port.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @port.draw_svg(svg: svg, x: x, y: y)
      @button.draw_svg(svg: svg, x: x + @button_offset, y: y) if @button
    end

    def control_files
      files = [@port.svg_file ]
      files += [:on, :off].map {|state| @button.svg_file(state: state)} if @button
      files
    end
  end

  class ToggleControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_texts = options[:labels]
      @selection = options.fetch(:selection, 1)
      @labels = Array(Label.new(module_: module_, text: label_texts.first, size: :small, alignment: :below))
      @labels << Label.new(module_: module_, text: label_texts[1], size: :small, alignment: :right_of) if (label_texts.size == 3)
      @labels << Label.new(module_: module_, text: label_texts.last, size: :small)
      @toggle = Toggle.new(module_: module_, size: label_texts.size)
    end

    def draw_faceplate(svg:, x:, y:)
      @labels.first.draw_svg(svg: svg, x: x, y: @toggle.bottom(y + PADDING))
      @labels[1].draw_svg(svg: svg, x: @toggle.right(x + PADDING / 2), y: y) if (@labels.size == 3)
      @labels.last.draw_svg(svg: svg, x: x, y: @toggle.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @toggle.draw_svg(svg: svg, x: x, y: y, selection: @selection)
    end

    def control_files
      (1..@toggle.size).map {|selection| @toggle.svg_file(selection: selection)}
    end
  end
end
