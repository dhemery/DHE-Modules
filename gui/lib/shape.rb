module DHE
  class Shape
    attr_reader :module_, :width, :height

    def initialize(module_:, top:, right:, bottom:, left:)
      @module_ = module_
      @top = top
      @right = right
      @bottom = bottom
      @left = left
      @width = right - left
      @height = bottom - top
    end

    def self.centered(width:, height: width)
      { left: -width / 2, right: width / 2, top: -height / 2, bottom: height / 2, }
    end

    def top(y)
      y + @top
    end

    def right(x)
      x + @right
    end

    def bottom(y)
      y + @bottom
    end

    def left(x)
      x + @left
    end

    def draw_bounding_box(svg:, x:, y:, color:)
      Shape::draw_box(svg: svg, top: top(y), right: right(x), bottom: bottom(y), left: left(x), color: color)
    end

    def self.draw_box(svg:, top:, right:, bottom:, left:, color:)
      svg.line(x1: left, x2: right, y1: top, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: right, x2: right, y1: bottom, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left, x2: right, y1: bottom, y2: bottom, 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left, x2: left, y1: bottom, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: color)
    end
  end

  require_relative 'box.rb'


  class RoundShape < Shape
    attr_reader :diameter

    def initialize(module_:, diameter:)
      super(module_: module_, **Shape::centered(width: diameter, height: diameter))
      @diameter = diameter
    end

    def radius
      diameter / 2
    end
  end
end
