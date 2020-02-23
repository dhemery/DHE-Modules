require_relative '../shapes/shape'

class Circle < Shape
  def initialize(radius:, fill:, stroke:, stroke_width:)
    super(top: -radius, right: radius, bottom: radius, left: -radius)
    @circle_attributes = {
      cx: 0,
      cy: 0,
      r: radius,
      fill: fill,
      stroke: stroke,
      'stroke-width' => stroke_width,
    }
  end

  def draw(canvas)
    canvas.circle(@circle_attributes)
  end
end
