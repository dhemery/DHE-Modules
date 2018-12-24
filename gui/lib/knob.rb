require_relative 'control'

module DHE
  class Knob < RoundControl
    DIAMETERS = {
        huge: 19.0,
        large: 12.7,
        medium: 10.0,
        small: 8.4,
        tiny: 7.0,
    }

    def initialize(spec:)
      @label = spec[:label]
      @style = spec[:style] || :large
      super(spec: spec, diameter: DIAMETERS[@style.to_sym])
    end

    def draw_foreground_svg(svg:, x:, y:, foreground:, background:)
      knob_color = foreground
      pointer_color = background
      pointer_width = radius / 8.0
      pointer_length = radius - pointer_width
      svg.g(transform: "translate(#{x} #{y})", stroke: pointer_color, fill: knob_color) do |g|
        g.circle(r: radius, stroke: 'none')
        g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
      end
    end
  end
end