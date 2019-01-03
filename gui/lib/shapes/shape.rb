require_relative 'dimensions'

module DHE
  class Shape
    attr_reader :faceplate, :width, :height, :top, :right, :bottom, :left, :x, :y

    def initialize(faceplate:, top:, right:, bottom:, left:, x: (right + left) / 2.0, y: (bottom + top) / 2.0)
      @faceplate = faceplate
      @top = top
      @right = right
      @bottom = bottom
      @left = left
      @width = right - left
      @height = bottom - top
      @x = x
      @y = y
    end

    def self.centered(x:, y:, width:, height: width)
      { left: x - width / 2, right: x + width / 2, top: y - height / 2, bottom: y + height / 2, }
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

  class RoundShape < Shape
    attr_reader :diameter

    def initialize(faceplate:, x:, y:, diameter:)
      super(faceplate: faceplate, **Shape::centered(x: x, y: y, width: diameter, height: diameter))
      @diameter = diameter
    end

    def radius
      diameter / 2
    end
  end
end
