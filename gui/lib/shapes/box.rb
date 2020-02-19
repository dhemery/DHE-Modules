require_relative '../dimensions'
require_relative 'shape'

class Box < BoundedShape
  CORNER_RADIUS = 1.0

  def initialize(top: 0, right: 0, bottom: 0, left: 0, fill:, stroke: 'none', stroke_width: STROKE_WIDTH, corner_radius: CORNER_RADIUS)
    super(top: top, right: right, bottom: bottom, left: left)
    @rect_attributes = {
      x: left,
      y: top,
      width: width,
      height: height,
      fill: fill,
      stroke: stroke,
      'stroke-width' => stroke_width,
      rx: corner_radius,
      ry: corner_radius
    }
  end

  def draw(canvas)
    canvas.rect(**@rect_attributes)
  end
end
