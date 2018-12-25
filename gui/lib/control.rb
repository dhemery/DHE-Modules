require_relative 'dimensions'
require_relative 'shape'

module DHE
  class Control
    attr_reader :name, :row, :column

    def initialize(name:, row:, column:)
      @name = name
      @row = row
      @column = column
    end

    def draw_image_svg(svg:, x:, y:)
      draw_panel_svg(svg: svg, x: x, y: y)
      draw_hardware_svg(svg: svg, x: x, y: y)
    end
  end

  class ButtonControl < Control
    DIAMETER = 6.0

    def initialize(panel:, control:)
      super(name: control[:name], row: control[:row], column: control[:column])
      text = control[:label]
      @style = control[:style]
      @label = Text.new(text: text, size: :small, alignment: :above, color: panel.foreground)
      @button = Button.new(foreground: panel.foreground, background: panel.background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @button.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @button.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class CounterControl < Control
    def initialize(panel:, control:)
      super(name: control[:name], row: control[:row], column: control[:column])
      @labels = control[:labels].map {|label| Text.new(text: label, color: panel.foreground, size: :small, alignment: :above)}
      @selection = control.fetch(:selection, 1)
      special = control[:special] || []
      @invisible = special.include? 'invisible'
      @button = Button.new(foreground: panel.foreground, background: panel.background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @labels[@selection - 1].draw_svg(svg: svg, x: x, y: @button.top(y: y)) unless @invisible
    end

    def draw_hardware_svg(svg:, x:, y:)
      @button.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class KnobControl < Control
    def initialize(panel:, control:)
      @style = control[:style] || :large
      super(name: control[:name], row: control[:row], column: control[:column])
      text = control[:label]
      @label = Text.new(text: text, size: :large, color: panel.foreground, alignment: :above)
      @knob = Knob.new(size: @style.to_sym, knob_color: panel.foreground, pointer_color: panel.background)
    end

    def draw_panel_svg(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @knob.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @knob.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class PortControl < Control
    def initialize(panel:, control:)
      super(name: control[:name], row: control[:row], column: control[:column])
      text = control[:label] || control[:style]
      @label = Text.new(text: text, size: :small, color: panel.foreground, alignment: :above)
      @port = Port.new(foreground: panel.foreground, background: panel.background)
    end

    def draw_panel_svg(x:, y:, svg:)
      @label.draw_svg(svg: svg, x: x, y: @port.top(y: y))
    end

    def draw_hardware_svg(svg:, x:, y:)
      @port.draw_svg(svg: svg, x: x, y: y)
    end
  end

  class ToggleControl < Control
    def initialize(panel:, control:)
      labels = control[:labels]
      foreground = panel.foreground
      background = panel.background
      super(name: control[:name], row: control[:row], column: control[:column])
      @labels = Array(Text.new(text: labels.first, size: :small, color: foreground, alignment: :below))
      @labels << Text.new(text: labels[1], size: :small, color: foreground, alignment: :right_of) if (labels.size == 3)
      @labels << Text.new(text: labels.last, size: :small, color: foreground, alignment: :above)
      @selection = control.fetch(:selection, 1)
      @toggle = ToggleShape.new(size: labels.size, foreground: foreground, background: background)
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
