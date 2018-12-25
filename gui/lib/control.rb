require_relative 'dimensions'

module DHE
  class Bounded
    attr_reader :width, :height

    def initialize(width:, height: width)
      @width = width
      @height = height
    end

    def left(x:)
      x - width / 2
    end

    def right(x:)
      x + width / 2
    end

    def top(y:)
      y - height / 2
    end

    def bottom(y:)
      y + height / 2
    end

    def draw_bounding_box_svg(svg:, x:, y:, color:)
      svg.line(x1: left(x: x), x2: right(x: x), y1: top(y: y), y2: top(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: right(x: x), x2: right(x: x), y1: bottom(y: y), y2: top(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left(x: x), x2: right(x: x), y1: bottom(y: y), y2: bottom(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left(x: x), x2: left(x: x), y1: bottom(y: y), y2: top(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
    end
  end

  class Control < Bounded
    attr_reader :name, :row, :column

    def initialize(spec:, width:, height:)
      super(width: width, height: height)
      @name = spec[:name]
      @row = spec[:row]
      @column = spec[:column]
    end

    def draw_image_svg(svg:, x:, y:, foreground:, background:)
      draw_background_svg(svg: svg, x: x, y: y, foreground: foreground, background: background)
      draw_foreground_svg(svg: svg, x: x, y: y, foreground: foreground, background: background)
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
