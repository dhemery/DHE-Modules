require_relative 'shape'

module DHE
  class Port < RoundShape
    DIAMETER = 8.4

    attr_reader :x, :y

    def initialize(faceplate:, x:, y:)
      super(faceplate: faceplate, x: x, y: y, diameter: DIAMETER)
      @foreground = faceplate.foreground
      @background = faceplate.background
      @path = faceplate.slug / 'port'
      @x = x
      @y = y
    end

    def svg_file
      SvgFile.new(path: @path, width: width, height: height) do |svg|
        draw_svg(svg: svg)
      end
    end

    def draw(svg:, x: @x, y: @y)
      stroke_width = DIAMETER * 0.025
      sleeve_diameter = DIAMETER - stroke_width
      step = sleeve_diameter / 7.0
      sleeve_radius = sleeve_diameter / 2.0
      ring_radius = sleeve_radius - step
      tip_radius = ring_radius - step
      svg.g(transform: "translate(#{x} #{y})", stroke: @foreground, fill: @background, 'stroke-width' => stroke_width) do |g|
        g.circle(r: sleeve_radius)
        g.circle(r: ring_radius)
        g.circle(r: tip_radius, fill: @foreground)
      end
    end

  end
end