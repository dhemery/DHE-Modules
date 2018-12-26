require_relative 'shape'

module DHE
  class Knob < RoundShape
    DIAMETERS = {
        huge: 19.0,
        large: 12.7,
        medium: 10.0,
        small: 8.4,
        tiny: 7.0,
    }

    def initialize(module_:, size:)
      super(module_: module_, diameter: DIAMETERS[size])
      @knob_color = module_.foreground
      @pointer_color = module_.background
      @path = module_.slug / "knob-#{size}"
    end

    def draw_svg(svg:, x:, y:)
      pointer_width = radius / 8.0
      pointer_length = radius - pointer_width
      svg.g(transform: "translate(#{x} #{y})", stroke: @pointer_color, fill: @knob_color) do |g|
        g.circle(r: radius, stroke: 'none')
        g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
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