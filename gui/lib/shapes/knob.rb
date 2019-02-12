require_relative 'shape'

class Knob < RoundShape
  DIAMETERS = { huge: 19.0, large: 12.7, medium: 10.0, small: 8.4, tiny: 7.0, }

  attr_reader :slug

  def initialize(size:, knob_color:, pointer_color:)
    super(DIAMETERS[size])
    @knob_color = knob_color
    @pointer_color = pointer_color
    @slug = Pathname("knob-#{size}")
  end

  def draw(canvas)
    pointer_width = radius / 8.0
    pointer_length = radius - pointer_width
    canvas.g(stroke: @pointer_color, fill: @knob_color) do |g|
      g.circle(r: radius, stroke: 'none')
      g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
    end
  end
end
