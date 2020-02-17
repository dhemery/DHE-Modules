require_relative '../shapes/shape'

class Circle < RoundShape
  def initialize(radius:, fill:, stroke: 'none', stroke_width: 0.0)
    super(radius * 2 + stroke_width)
    @radius = radius
    @fill = fill
    @stroke = stroke
    @stroke_width = stroke_width
  end

  def draw(canvas)
    canvas.circle(cx: 0, cy: 0, r: @radius, fill: @fill, stroke: @stroke, 'stroke-width' => @stroke_width)
  end
end
