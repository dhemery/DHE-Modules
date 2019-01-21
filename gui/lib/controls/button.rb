require_relative '../control'

class ButtonState
  attr_reader :slug, :control

  def initialize(control:, slug:, fill:, stroke:)
    @control = control
    @slug = slug
    @fill = fill
    @stroke = stroke
  end

  def draw(svg:, x:, y:)
    diameter = control.diameter
    stroke_width = diameter / 6.0
    circle_diameter = diameter - stroke_width
    circle_radius = circle_diameter / 2.0
    svg.circle(cx: x, cy: y, r: circle_radius,
               'stroke-width' => stroke_width, fill: @fill, stroke: @stroke)
  end
end

class Button < RoundControl
  DIAMETER = 6.0
  SLUGS = {
      normal: 'button',
      reversed: 'button-reversed'
  }

  def initialize(x:, y:, ring_color:, pressed_color:, style:)
    super(x: x, y: y, diameter: DIAMETER)
    slug = SLUGS[style]
    @states = [
        ButtonState.new(
            control: self,
            slug: "#{slug}-1",
            stroke: ring_color,
            fill: ring_color
        ),
        ButtonState.new(
            control: self,
            slug: "#{slug}-2",
            stroke: ring_color,
            fill: pressed_color
        )
    ]
    @default_state = @states.first
  end
end