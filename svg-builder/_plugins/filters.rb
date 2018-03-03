module Jekyll
  module Filters
    PIXELS_PER_MM = 2.9527559055
    def to_pixels(mm)
      mm * PIXELS_PER_MM
    end
    module_function :to_pixels

    class Port
      RADIUS = Filters::to_pixels(4.2)
      DIAMETER = RADIUS*2

      attr_reader :x, :y, :radius, :diameter

      def initialize(x, y)
        @x = x
        @y = y
      end

      def top
        @y - RADIUS
      end

      def left
        @x - RADIUS
      end

      def right
        @x + RADIUS
      end

      def width
        DIAMETER
      end

      def bottom
        @y + RADIUS
      end
    end

    class SmallLabel
      ASCENT_RATIO = 0.66666666666
      FONT_SIZE = 7.0 # pixels
      HEIGHT = FONT_SIZE*ASCENT_RATIO

      def initialize(center_x, bottom, text, color)
        @center_x = center_x
        @bottom = bottom
        @text = text
        @color = color
      end

      def top
        @bottom - HEIGHT
      end

      def to_liquid
        %Q[<text text-anchor="middle" fill="#{@color}" x="#{@center_x}" y="#{@bottom}" style="font-family:Proxima Nova;font-weight:bold;font-size:#{FONT_SIZE}px">#{@text}</text>]
      end
    end

    class Box
      STROKE_WIDTH = Filters::to_pixels(0.5)
      PADDING = Filters::to_pixels(1.0)
      STROKE_INSET = STROKE_WIDTH / 2
      CORNER_RADIUS = STROKE_WIDTH * 2

      def initialize(top, right, bottom, left, border, background)
        @top = top - PADDING - STROKE_INSET
        @left = left - PADDING - STROKE_INSET
        @right = right + PADDING + STROKE_INSET
        @bottom = bottom + PADDING + STROKE_INSET
        @width = @right - @left
        @height = @bottom - @top
        @border = border
        @background = background
      end

      def to_liquid
        %Q[
        <rect x="#{@left}" y="#{@top}" width="#{@width}" height="#{@height}"
          stroke-width="#{STROKE_WIDTH}" rx="#{CORNER_RADIUS}" ry="#{CORNER_RADIUS}"
          stroke="#{@border}" fill="#{@background}"/>
        ]
      end
    end

    class PortBox
      def initialize(x, y, border, background, label_text, label_color)
        port = Port.new(x, y)
        @label = SmallLabel.new(x, port.top - Box::PADDING, label_text, label_color)
        @box = Box.new(@label.top, port.right, port.bottom, port.left, border, background)
      end

      def to_liquid
        @box.to_liquid + @label.to_liquid
      end
    end

    def port_box(x_mm, y_mm, border, background, label, label_color)
      x_px = Filters::to_pixels(x_mm)
      y_px = Filters::to_pixels(y_mm)
      PortBox.new(x_px, y_px, border, background, label, label_color)
    end

    def cv_port_box(page, x_mm, y_mm)
      port_box(x_mm, y_mm, "none", "none", "CV", page["dark_color"])
    end

    def in_port_box(page, x_mm, y_mm, label)
      port_box(x_mm, y_mm, page["dark_color"], page["light_color"], label, page["dark_color"])
    end

    def out_port_box(page, x, y, label)
      port_box(x, y, page["dark_color"], page["dark_color"], label, page["light_color"])
    end
  end
end

Liquid::Template.register_filter(Jekyll::Filters)
