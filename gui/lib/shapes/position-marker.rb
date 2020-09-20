require_relative 'shape'

class PositionMarker < Shape
  attr_reader :slug
  TOP = -Light::DIAMETER
  BOTTOM = Light::DIAMETER
  HEIGHT = BOTTOM - TOP
  RIGHT = Light::DIAMETER
  LEFT = -Light::DIAMETER
  NIB = Light::DIAMETER
  THICKNESS = STROKE_WIDTH

  def initialize(type:, color:)
    @color = color
    @slug = Pathname("marker-#{type}")
    @outer = type == :start ? LEFT : RIGHT
    @inner = type == :start ? LEFT + NIB : RIGHT - NIB
    super(top: TOP, right: RIGHT, bottom: BOTTOM, left: LEFT)
  end

  def draw(canvas)
    canvas.line(x1: @outer, y1: TOP, x2: @outer, y2: BOTTOM, 'stroke-width' => THICKNESS, 'stroke-linecap' => "square", stroke: @color)
    canvas.line(x1: @outer, y1: TOP, x2: @inner, y2: TOP, 'stroke-width' => THICKNESS, 'stroke-linecap' => "square", stroke: @color)
    canvas.line(x1: @outer, y1: BOTTOM, x2: @inner, y2: BOTTOM, 'stroke-width' => THICKNESS, 'stroke-linecap' => "square", stroke: @color)
  end

  def frames
    [self]
  end
end
