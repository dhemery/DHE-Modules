require_relative '../shapes/shape'

class Circle < RoundShape
  def initialize(radius:, fill:, stroke: 'none', stroke_width: 0.0)
    super(radius * 2 + stroke_width)
    @circle_attributes = {
      cx: 0,
      xy: 0,
      r: radius,
      fill: fill,
      stroke: stroke,
      'stroke-width' => stroke_width
    }
  end

  def draw(canvas)
    canvas.circle(@circle_attributes)
  end
end
