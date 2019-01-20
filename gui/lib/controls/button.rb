require_relative '../control'

class Button < RoundControl
  DIAMETER = 6.0

  def initialize(x:, y:, ring_color:, pressed_color:, style:)
    super(x: x, y: y, diameter: DIAMETER)
    @slug = 'button'
    @slug += '-reversed' if style == :reversed
    @states = [
        {
            slug: "#{@slug}-1",
            stroke: ring_color,
            fill: ring_color
        },
        {
            slug: "#{@slug}-2",
            stroke: ring_color,
            fill: pressed_color
        }
    ]
    @default_state = @states[0]
  end

  def draw_faceplate(svg:)
    draw(svg: svg, x: @x, y: @y, **@default_state)
  end

  def draw(svg:, x:, y:, **state)
    stroke_width = DIAMETER / 6.0
    circle_diameter = DIAMETER - stroke_width
    circle_radius = circle_diameter / 2.0
    svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: state[:fill], stroke: state[:stroke])
  end

  def svg_files(dir)
    @states.map do |state|
      path = dir / state[:slug]
      svg_file(path: path) do |svg|
        draw_control(svg: svg, **state)
      end
    end
  end
end