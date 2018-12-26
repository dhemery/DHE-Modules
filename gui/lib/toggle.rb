require_relative 'shape'

module DHE
  class Toggle < Shape
    WIDTH = 3.0

    attr_reader :size

    def initialize(module_:, size:)
      super(module_: module_, **Shape::centered(width: WIDTH, height: size * WIDTH))
      @size = size
      @foreground = module_.foreground
      @background = module_.background
    end

    def draw_svg(svg:, x:, y:, selection:)
      position =
          case selection
          when @size
            1.0
          when 1
            -1.0
          else
            0.0
          end
      box_stroke_width = width / 8.0
      interior_inset = box_stroke_width / 2.0

      box_width = width - box_stroke_width
      box_height = height - box_stroke_width
      box_left = -width / 2.0 + interior_inset
      box_top = -height / 2.0 + interior_inset

      interior_width = box_width - box_stroke_width
      interior_height = box_height - box_stroke_width
      corner_radius = interior_inset

      knurl_stroke_width = 0.25
      knurl_inset = knurl_stroke_width * 2.0
      knurl_length = interior_width - knurl_inset
      knurl_left = knurl_length / -2.0
      knurl_right = knurl_left + knurl_length
      knurl_spacing = knurl_stroke_width * 2.0

      lever_height = knurl_spacing * 4.0 + knurl_stroke_width
      lever_inset = knurl_stroke_width
      lever_distance = (interior_height - lever_height) / 2.0 - lever_inset
      lever_offset = lever_distance * -position

      svg.g(transform: "translate(#{x} #{y})", fill: @background, stroke: @foreground) do |g|
        g.rect(x: box_left, y: box_top, width: box_width, height: box_height,
               rx: corner_radius, ry: corner_radius, 'stroke-width' => box_stroke_width)
        (-2..2).map {|index| knurl_spacing * index + lever_offset}.each do |knurl_y|
          g.line(x1: knurl_left, x2: knurl_right, y1: knurl_y, y2: knurl_y,
                 'stroke-width' => knurl_stroke_width, 'stroke-linecap' => 'round')
        end
      end
    end

    def svg_file(selection:)
      path = @module_.slug / "toggle-#{@size}-#{selection}"
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        draw_svg(svg: svg, x: width / 2, y: height / 2, selection: selection)
      end
      SvgFile.new(path: path, content: content)
    end
  end
end