require_relative '../dimensions'
require_relative '../control'

class Box < Control
  CORNER_RADIUS = 1.0

  def initialize(top:, right:, bottom:, left:, stroke:, fill:)
    super(top: top, right: right, bottom: bottom, left: left)
    @stroke = stroke
    @fill = fill
  end

  def draw_faceplate(svg:)
    svg.rect(x: left, y: top, width: @width, height: @height,
             rx: CORNER_RADIUS, ry: CORNER_RADIUS,
             stroke: @stroke, fill: @fill, 'stroke-width' => STROKE_WIDTH)
  end
end