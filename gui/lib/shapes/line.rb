require_relative 'shape'
require_relative '../dimensions'

class Line < BoundedShape
  def initialize(left:, right:, y:, color:)
    super(top: y, right: right, bottom: y, left: left)
  end

  def draw(canvas)
    canvas.line(x1: left, y1: top, x2: right, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: @stroke)
  end
end