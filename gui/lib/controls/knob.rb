require_relative '../control'

class Knob < RoundControl
  DIAMETERS = { huge: 19.0, large: 12.7, medium: 10.0, small: 8.4, tiny: 7.0, }

  def initialize(x:, y:, size:, knob_color:, pointer_color:)
    super(x: x, y: y, diameter: DIAMETERS[size.to_sym])
    @knob_color = knob_color
    @pointer_color = pointer_color
    @slug = "knob-#{size}"
    @x = x
    @y = y
  end

  def draw(svg:, x:, y:)
    pointer_width = radius / 8.0
    pointer_length = radius - pointer_width
    svg.g(transform: "translate(#{x} #{y})", stroke: @pointer_color, fill: @knob_color) do |g|
      g.circle(r: radius, stroke: 'none')
      g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
    end
  end

  def svg_files(dir)
    [
        svg_file(path: dir / @slug) do |svg|
          draw_control(svg: svg)
        end
    ]
  end
end