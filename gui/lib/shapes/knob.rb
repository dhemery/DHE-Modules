require_relative 'shape'

module DHE
  class Knob < RoundShape
    DIAMETERS = { huge: 19.0, large: 12.7, medium: 10.0, small: 8.4, tiny: 7.0, }

    def initialize(faceplate:, size:, x:, y:)
      super(faceplate: faceplate, x: x, y: y, diameter: DIAMETERS[size.to_sym])
      @knob_color = faceplate.foreground
      @pointer_color = faceplate.background
      @path = faceplate.slug / "knob-#{size}"
      @x = x
      @y = y
    end

    def svg_file
      SvgFile.new(path: @path, width: width, height: height) do |svg|
        draw_svg(svg: svg)
      end
    end

    def draw(svg:, x: @x, y: @y)
      pointer_width = radius / 8.0
      pointer_length = radius - pointer_width
      svg.g(transform: "translate(#{x} #{y})", stroke: @pointer_color, fill: @knob_color) do |g|
        g.circle(r: radius, stroke: 'none')
        g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
      end
    end
  end
end