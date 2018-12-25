require_relative 'control'

module DHE
  class Toggle < Control
    WIDTH = 3.0

    def initialize(spec:)
      labels = spec[:labels]
      super(spec: spec, width: WIDTH, height: WIDTH * labels.size)
      @labels = Array(Text.new(text: labels.first, size: :small, alignment: :below))
      @labels << Text.new(text: labels[1], size: :small, alignment: :right_of) if (labels.size == 3)
      @labels << Text.new(text: labels.last, size: :small, alignment: :above)
      @position = (spec[:position] || 1) - 1
    end

    def draw_foreground_svg(svg:, x:, y:, foreground:, background:)
      position = @position - 1

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

      svg.g(transform: "translate(#{x} #{y})", fill: background, stroke: foreground) do |g|
        g.rect(x: box_left, y: box_top, width: box_width, height: box_height,
               rx: corner_radius, ry: corner_radius, 'stroke-width' => box_stroke_width)
        (-2..2).map {|index| knurl_spacing * index + lever_offset}.each do |knurl_y|
          g.line(x1: knurl_left, x2: knurl_right, y1: knurl_y, y2: knurl_y,
                 'stroke-width' => knurl_stroke_width, 'stroke-linecap' => 'round')
        end
      end
    end

    def draw_background_svg(x:, y:, svg:, foreground:, background:)
      @labels.first.draw_svg(svg: svg, x: x, y: bottom(y: y), color: foreground)
      @labels[1].draw_svg(svg: svg, x: right(x: x), y: y, color: foreground) if (@labels.size == 3)
      @labels.last.draw_svg(svg: svg, x: x, y: top(y: y), color: foreground)
    end
  end
end
