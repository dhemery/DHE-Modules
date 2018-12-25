module DHE
  class Shape
    attr_reader :width, :height

    def initialize(width:, height: width)
      @width = width
      @height = height
    end

    def left(x:)
      x - width / 2
    end

    def right(x:)
      x + width / 2
    end

    def top(y:)
      y - height / 2
    end

    def bottom(y:)
      y + height / 2
    end

    def draw_bounding_box_svg(svg:, x:, y:, color:)
      svg.line(x1: left(x: x), x2: right(x: x), y1: top(y: y), y2: top(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: right(x: x), x2: right(x: x), y1: bottom(y: y), y2: top(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left(x: x), x2: right(x: x), y1: bottom(y: y), y2: bottom(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left(x: x), x2: left(x: x), y1: bottom(y: y), y2: top(y: y), 'stroke-width' => STROKE_WIDTH, stroke: color)
    end
  end

  class RoundShape < Shape
    attr_reader :diameter

    def initialize(diameter:)
      super(width: diameter, height: diameter)
      @diameter = diameter
    end

    def radius
      diameter / 2
    end
  end

  class Button < RoundShape
    DIAMETER = 6.0

    def initialize(foreground:, background:)
      super(diameter: DIAMETER)
      @foreground = foreground
      @background = background
    end

    def draw_svg(svg:, x:, y:, state: :off, style: :normal)
      ring_color = @foreground
      center_color = state == :on ? @background : @foreground
      ring_color, center_color = center_color, ring_color if style == :reversed
      stroke_width = DIAMETER / 6.0
      circle_diameter = DIAMETER - stroke_width
      circle_radius = circle_diameter / 2.0
      svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: center_color, stroke: ring_color)
    end
  end

  class Knob < RoundShape
    DIAMETERS = {
        huge: 19.0,
        large: 12.7,
        medium: 10.0,
        small: 8.4,
        tiny: 7.0,
    }

    def initialize(size:, knob_color:, pointer_color:)
      super(diameter: DIAMETERS[size])
      @knob_color = knob_color
      @pointer_color = pointer_color

    end

    def draw_svg(svg:, x:, y:)
      pointer_width = radius / 8.0
      pointer_length = radius - pointer_width
      svg.g(transform: "translate(#{x} #{y})", stroke: @pointer_color, fill: @knob_color) do |g|
        g.circle(r: radius, stroke: 'none')
        g.line(y2: -pointer_length, 'stroke-width' => pointer_width, 'stroke-linecap' => 'round')
      end
    end
  end

  class Label < Shape
    BASELINES = {
        above: 'alphabetic',
        below: 'hanging',
        right_of: 'middle'
    }
    ANCHORS = {
        above: 'middle',
        below: 'middle',
        right_of: 'start'
    }
    ASCENT_RATIO = 2.0 / 3.0 # Approximately correct for Proxima Nova font

    SIZES = {
        panel: 12.0 / PX_PER_MM,
        large: 9.0 / PX_PER_MM,
        small: 7.0 / PX_PER_MM
    }

    def initialize(text:, size:, color:, alignment:, padding: PADDING)
      @text = text&.upcase || ''
      @size = SIZES[size.to_sym]
      super(height: @size, width: @text.length * @size * 0.6)
      @color = color
      @alignment = alignment
      @baseline = BASELINES[@alignment]
      @anchor = ANCHORS[@alignment]
      @padding = padding
    end

    def draw_svg(svg:, x:, y:)
      # draw_bounding_box_svg(svg: svg, x: x, y: y, color: color)

      case @alignment
      when :above
        y -= @padding
      when :below
        y += @padding
      when :right_of
        x += @padding / 2
      end

      svg.text(@text, x: x, y: y,
               'dominant-baseline' => @baseline, 'text-anchor' => @anchor, fill: @color,
               style: "font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px")
    end
  end

  class Port < RoundShape
    DIAMETER = 8.4

    def initialize(foreground:, background:)
      super(diameter: DIAMETER)
      @foreground = foreground
      @background = background
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
  end

  class Toggle < Shape
    WIDTH = 3.0

    def initialize(size:, foreground:, background:)
      super(width: WIDTH, height: size * WIDTH)
      @size = size
      @foreground = foreground
      @background = background
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
  end
end
