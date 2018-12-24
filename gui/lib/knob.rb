require_relative 'control'

module DHE
  class Knob < Control
    DIAMETERS = {
        huge: 19.0,
        large: 12.7,
        medium: 10.0,
        small: 8.4,
        tiny: 7.0,
    }

    attr_reader :radius

    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @label = spec[:label]
      @style = spec[:style] || :large
      @radius = DIAMETERS[@style.to_sym] / 2
    end

    def to_s
      "#{super} #{@style} knob (#{@label})"
    end

    def draw_on_image(panel:, svg:)
      x = panel.x(column)
      y = panel.y(row)
      draw_inner_svg(panel: panel, x: x, y: y, svg: svg)
    end

    def draw_inner_svg(panel:, x:, y:, svg:)
      knob_color = panel.foreground
      pointer_color = panel.background
      pointer_width = radius / 8.0
      pointer_length = radius - pointer_width
      svg.g(transform: "translate(#{x} #{y})", stroke: pointer_color, fill: knob_color) do |g|
        g.circle(r: radius, stroke: 'none')
        g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
      end
    end
  end
end