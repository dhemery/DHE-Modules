require_relative '../dimensions'
require_relative 'shape'

class Box < Shape
  CORNER_RADIUS = 1.0

  def initialize(top:, right:, bottom:, left:, fill:, stroke:, stroke_width:, corner_radius: CORNER_RADIUS)
    inset = stroke_width / 2.0
    super(top: top - inset, right: right + inset, bottom: bottom + inset, left: left - inset)
    @rect_attributes = {
      x: left,
      y: top,
      width: right - left,
      height: bottom - top,
      fill: fill,
      stroke: stroke,
      'stroke-width' => stroke_width,
      rx: corner_radius,
      ry: corner_radius,
    }
  end

  def draw(canvas)
    canvas.rect(**@rect_attributes)
  end

  def self.centered(width: 0, height: 0, fill: nil, stroke: nil, stroke_width: nil, corner_radius: CORNER_RADIUS)
    right = width / 2
    bottom = height / 2
    Box.new(top: -bottom, right: right, bottom: bottom, left: -right,
            fill: fill, stroke: stroke, stroke_width: stroke_width, corner_radius: corner_radius)
  end

  def self.around(shapes:, fill:, stroke:, stroke_width:, corner_radius: CORNER_RADIUS)
    Box.new(
      top: shapes.map(&:top).min,
      right: shapes.map(&:right).max,
      bottom: shapes.map(&:bottom).max,
      left: shapes.map(&:left).min,
      fill: fill,
      stroke: stroke,
      stroke_width: stroke_width,
      corner_radius: corner_radius
    )
  end
end
