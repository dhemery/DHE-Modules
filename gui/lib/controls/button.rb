require_relative '../control'

class Button < RoundControl
  DIAMETER = 6.0
  SLUGS = {
      normal: 'button',
      reversed: 'button-reversed'
  }

  def initialize(x:, y:, ring_color:, pressed_color:, style:)
    super(slug: SLUGS[style], x: x, y: y, diameter: DIAMETER)
    @states = [
        {
            slug: "#{slug}-1",
            stroke: ring_color,
            fill: ring_color
        },
        {
            slug: "#{slug}-2",
            stroke: ring_color,
            fill: pressed_color
        }
    ]
    @default_state = @states[0]
  end

  def draw(svg:, x:, y:, **state)
    stroke_width = DIAMETER / 6.0
    circle_diameter = DIAMETER - stroke_width
    circle_radius = circle_diameter / 2.0
    svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: state[:fill], stroke: state[:stroke])
  end
end