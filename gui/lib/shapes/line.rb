require_relative 'shape'
require_relative '../dimensions'

class Line < BoundedShape
  def initialize(x1: 0, y1: 0, x2: x1, y2: y1, stroke:, width: STROKE_WIDTH, cap: 'square')
    super(top: [y1, y2].min, right: [x1, x2].max, bottom: [y1, y2].max, left: [x1, x2].min)
    @line_attributes = {
      x1: x1, y1: y1,
      x2: x2, y2: y2,
      stroke: stroke, 'stroke-width' => width, 'stroke-linecap' => cap
    }
  end

  def draw(canvas)
    canvas.line(**@line_attributes)
  end
end
