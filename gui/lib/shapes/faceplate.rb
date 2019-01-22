require_relative '../dimensions'
require_relative 'shape'

class Faceplate < BoundedShape
  CORNER_RADIUS = 1.0

  def initialize(top:, right:, bottom:, left:, stroke:, fill:)
    super(top: top, right: right, bottom: bottom, left: left)
    @stroke = stroke
    @fill = fill
  end

  def draw(canvas)
    canvas.rect(x: 0, y: 0, width: @width, height: @height,
           stroke: @foreground, fill: @background, 'stroke-width' => 1)
  end
end
