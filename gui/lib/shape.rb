module DHE
  class Shape
    attr_reader :module_, :width, :height

    def initialize(module_:, top:, right:, bottom:, left:)
      @module_ = module_
      @top = top
      @right = right
      @bottom = bottom
      @left = left
      @width = right - left
      @height = bottom - top
    end

    def self.centered(width:, height: width)
      {
          left: -width / 2,
          right: width / 2,
          top: -height / 2,
          bottom: height / 2,
      }
    end

    def top(y)
      y + @top
    end

    def right(x)
      x + @right
    end

    def bottom(y)
      y + @bottom
    end

    def left(x)
      x + @left
    end

    def draw_bounding_box(svg:, x:, y:, color:)
      Shape::draw_box(svg: svg, top: top(y), right: right(x), bottom: bottom(y), left: left(x), color: color)
    end

    def self.draw_box(svg:, top:, right:, bottom:, left:, color:)
      svg.line(x1: left, x2: right, y1: top, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: right, x2: right, y1: bottom, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left, x2: right, y1: bottom, y2: bottom, 'stroke-width' => STROKE_WIDTH, stroke: color)
      svg.line(x1: left, x2: left, y1: bottom, y2: top, 'stroke-width' => STROKE_WIDTH, stroke: color)
    end
  end

  class Box < Shape
    CORNER_RADIUS = 1.0
    BUFFER = PADDING + STROKE_INSET

    def initialize(module_:, top:, right:, bottom:, left:, style: :normal)
      super(module_: module_, top: top - BUFFER, right: right + BUFFER, bottom: bottom + BUFFER, left: left - BUFFER)
      @stroke = module_.foreground
      @fill = style == :normal ? module_.background : @stroke
    end

    def draw_svg(svg:, x:, y:)
      svg.rect(x: @left + x, y: @top + y, width: @width, height: @height,
               'stroke-width' => STROKE_WIDTH, rx: CORNER_RADIUS, ry: CORNER_RADIUS,
               stroke: @stroke, fill: @fill)
    end
  end

  class RoundShape < Shape
    attr_reader :diameter

    def initialize(module_:, diameter:)
      super(module_: module_, **Shape::centered(width: diameter, height: diameter))
      @diameter = diameter
    end

    def radius
      diameter / 2
    end
  end

  class Button < RoundShape
    DIAMETER = 6.0

    def initialize(module_:, style: :normal)
      super(module_: module_, diameter: DIAMETER)
      @style = style
      @ring_color = module_.foreground
      @center_color = module_.background
      @ring_color, @center_color = @center_color, @ring_color if @style == :reversed
    end

    def draw_svg(svg:, x:, y:, state: :off)
      center_color = state == :on ? @center_color : @ring_color
      stroke_width = DIAMETER / 6.0
      circle_diameter = DIAMETER - stroke_width
      circle_radius = circle_diameter / 2.0
      svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: center_color, stroke: @ring_color)
    end

    def svg_file(state:)
      position = state == :off ? 1 : 2
      path = module_.slug / "button-#{@style}-#{position}"
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        draw_svg(svg: svg, x: width / 2, y: height / 2, state: state)
      end
      SvgFile.new(path: path, content: content)
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

    def initialize(module_:, size:)
      super(module_: module_, diameter: DIAMETERS[size])
      @size = size
      @knob_color = module_.foreground
      @pointer_color =  module_.background

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
      path = module_.slug / "knob-#{@size}"
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        draw_svg(svg: svg, x: width / 2, y: height / 2)
      end
      SvgFile.new(path: path, content: content)
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

    attr_reader :text

    def initialize(module_:, text:, size:, style: :normal, alignment: :above)
      @text = text&.upcase || ''
      @size = SIZES[size.to_sym]
      @color = style == :normal ? module_.foreground : module_.background
      @alignment = alignment
      @baseline = BASELINES[@alignment]
      @anchor = ANCHORS[@alignment]
      height = @size * ASCENT_RATIO
      width = @text.length * @size * 0.6 # Approximate
      left = case alignment
             when :right_of
               PADDING / 2
             else
               -width / 2
             end
      top = case alignment
            when :above
              -(height + PADDING)
            when :right_of
              -height / 2
            else
              PADDING
            end
      bottom = top + height
      right = left + width
      super(module_: module_, top: top, right: right, bottom: bottom, left: left)
    end

    def draw_svg(svg:, x:, y:)
      # draw_bounding_box(svg: svg, x: x, y: y, color: @color)

      svg.text(@text, x: x, y: y,
               'dominant-baseline' => @baseline, 'text-anchor' => @anchor, fill: @color,
               style: "font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px")
    end
  end

  class Port < RoundShape
    DIAMETER = 8.4

    def initialize(module_:)
      super(module_: module_, diameter: DIAMETER)
      @foreground = module_.foreground
      @background = module_.background
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
      path = module_.slug / "port"
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        draw_svg(svg: svg, x: width / 2, y: height / 2)
      end
      SvgFile.new(path: path, content: content)
    end
  end

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
