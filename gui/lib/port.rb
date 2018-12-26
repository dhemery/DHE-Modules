require_relative 'shape'

module DHE
  class Port < RoundShape
    DIAMETER = 8.4

    def initialize(module_:)
      super(module_: module_, diameter: DIAMETER)
      @foreground = module_.foreground
      @background = module_.background
      @path = module_.slug / 'port'
    end

    def draw_svg(svg:, x:, y:)
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

    def svg_file
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        draw_svg(svg: svg, x: width / 2, y: height / 2)
      end
      SvgFile.new(path: @path, content: content)
    end
  end
end