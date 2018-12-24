module DHE
  class Control
    attr_reader :name, :row, :column, :width, :height

    def initialize(spec:, width:, height:)
      @name = spec[:name]
      @row = spec[:row]
      @column = spec[:column]
      @width = width
      @height = height
    end

    def draw_image_svg(svg:, x:, y:, foreground:, background:)
      draw_background_svg(svg: svg, x: x, y: y, foreground: foreground, background: background)
      draw_foreground_svg(svg: svg, x: x, y: y, foreground: foreground, background: background)
    end

    def draw_background_svg(svg:, x:, y:, foreground:, background:)
    end
  end

  class RoundControl < Control
    attr_reader :diameter

    def initialize(spec:, diameter:)
      @diameter = diameter
      super(spec: spec, width: @diameter, height: @diameter)
    end

    def radius
      diameter / 2
    end
  end
end
