require_relative 'shape'
require_relative '../dimensions'

class Line < BoundedShape
  def initialize(x1:, y1:, x2:, y2:, color:)
    super(top: [y1, y2].min, right: [x1, x2].max, bottom: [y1, y2].max, left: [x1, x2].min)
    @x1 = x1
    @y1 = y1
    @x2 = x2
    @y2 = y2
    @color = color
  end

  def draw(canvas)
    canvas.line(x1: @x1, y1: @y1, x2: @x2, y2: @y2, 'stroke-width' => STROKE_WIDTH, stroke: @color)
  end
end