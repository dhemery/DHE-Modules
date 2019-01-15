require_relative '../control'

class Button < RoundControl
  DIAMETER = 6.0

  def initialize(x:, y:, ring_color:, pressed_color:, style: :normal)
    super(x: x, y: y, diameter: DIAMETER)
    @slug = 'button'
    @slug += '-reversed' if style == :reversed
    @ring_color = ring_color
    @pressed_color = pressed_color
  end

  def draw(svg:, x:, y:, state: :off)
    state_color = state == :on ? @pressed_color : @ring_color
    stroke_width = DIAMETER / 6.0
    circle_diameter = DIAMETER - stroke_width
    circle_radius = circle_diameter / 2.0
    svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: state_color, stroke: @ring_color)
  end

  def svg_files(dir)
    [:on, :off].map do |state|
      position = state == :off ? 1 : 2
      path = dir / "#{@slug}-#{position}"
      svg_file(path: path) do |svg|
        draw_control(svg: svg, state: state)
      end
    end
  end
end