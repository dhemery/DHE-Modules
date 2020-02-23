require_relative 'shape'
require_relative '../dimensions'

class Line < Shape
  def initialize(x1:, y1:, x2:, y2:, stroke:, width:, cap: nil)
    super(top: [y1, y2].min, right: [x1, x2].max, bottom: [y1, y2].max, left: [x1, x2].min)
    @line_attributes = {
      x1: x1, y1: y1,
      x2: x2, y2: y2,
      stroke: stroke, 'stroke-width' => width
    }
    @line_attributes['stroke-linecap'] = cap unless cap.nil?
  end

  def draw(canvas)
    canvas.line(**@line_attributes)
  end
end
