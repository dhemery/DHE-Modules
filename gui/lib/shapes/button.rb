require_relative 'shape'

class Button < RoundShape
  DIAMETER = 6.0

  attr_reader :slug

  def initialize(foreground:, background:, style: :normal, state:)
    super(DIAMETER)
    foreground, background = background, foreground unless style == :normal
    @stroke = foreground
    @fill = state == :pressed ? background : foreground
    style_slug = style == :reversed ? '-reversed' : ''
    state_slug = state == :pressed ? '-2' : '-1'
    @slug = "button#{style_slug}#{state_slug}"
  end

  def draw(canvas)
    stroke_width = diameter / 6.0
    circle_diameter = diameter - stroke_width
    circle_radius = circle_diameter / 2.0
    canvas.circle(cx: 0, cy: 0, r: circle_radius,
                  'stroke-width' => stroke_width, fill: @fill, stroke: @stroke)
  end
end
