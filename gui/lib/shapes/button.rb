require_relative 'shape'

class Button < RoundShape
  DIAMETER = 6.0

  def initialize(stroke:, fill:, style: :normal)
    super(DIAMETER)
    @stroke = stroke
    @fill = fill
  end

  def draw(canvas)
    stroke_width = diameter / 6.0
    circle_diameter = diameter - stroke_width
    circle_radius = circle_diameter / 2.0
    canvas.circle(cx: 0, cy: 0, r: circle_radius,
                  'stroke-width' => stroke_width, fill: @fill, stroke: @stroke)
  end
end