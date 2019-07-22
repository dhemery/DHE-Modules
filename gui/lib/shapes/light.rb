require_relative 'shape'
require_relative '../dimensions'

class Light < RoundShape
  RADIUS = 1.088

  def initialize(color:)
    super(RADIUS * 2.0)
    @stroke = @fill = color
  end

  def draw(canvas)
    canvas.circle(cx: 0, cy: 0, r: radius,
                  'stroke-width' => 0.0, fill: @fill, stroke: @stroke)
  end
end