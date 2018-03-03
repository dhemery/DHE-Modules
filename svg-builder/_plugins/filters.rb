module Jekyll
  module Filters
    def to_pixels(mm)
      mm * 2.9527559055
    end
    module_function :to_pixels

    class Port
      attr_reader :x, :y, :radius, :diameter

      def initialize(x, y)
        @x = Filters::to_pixels(x)
        @y = Filters::to_pixels(y)
        @radius = Filters::to_pixels(4.2)
        @diameter = @radius * 2
      end

      def left
        @x - @radius
      end

      def top
        @y - @radius
      end
    end

    class SmallLabel
      attr_reader :text, :font_size, :height

      def initialize(text)
        @text = text
        @font_size = 7.0 #pixels
        ascent_ratio = 0.667
        @height = @font_size * ascent_ratio
      end
    end

    class PortBox
      def initialize(port, border, background, label, label_color)
        @port = port
        @border = border
        @background = background
        @label = label
        @label_color = label_color
        @padding = Filters::to_pixels(1.0)
        @stroke_width = Filters::to_pixels(0.5)
        @stroke_inset = @stroke_width / 2
        @corner_radius = @stroke_width * 2
      end

      def left
        @port.left - @padding - @stroke_inset
      end

      def top
        @port.top - @padding - @label.height - @padding - @stroke_inset
      end

      def width
        @stroke_inset + @padding + @port.diameter + @padding + @stroke_inset
      end

      def height
        @stroke_inset + @padding + @label.height + @padding + @port.diameter + @padding + @stroke_inset
      end

      def to_liquid
        %Q[
        <rect x="#{left}" y="#{top}" width="#{width}" height="#{height}"
          stroke-width="#{@stroke_width}" rx="#{@corner_radius}" ry="#{@corner_radius}"
          stroke="#{@border}" fill="#{@background}"/>
          <text text-anchor="middle" fill="#{@label_color}" x="#{@port.x}" y="#{@port.top - @padding}" style="font-family:Proxima Nova;font-weight:bold;font-size:#{@label.font_size}px">#{@label.text}</text>
        ]
      end
    end

    def port_box(label, x, y, border_color, background_color, label_color)
      port = Port.new(x, y)
      label = SmallLabel.new(label)
      PortBox.new(port, border_color, background_color, label, label_color)
    end

    def in_port_box(label, x, y, page)
      port_box(label, x, y, page["dark_color"], page["light_color"], page["dark_color"])
    end

    def out_port_box(label, x, y, page)
      port_box(label, x, y, page["dark_color"], page["dark_color"], page["light_color"])
    end
  end
end

Liquid::Template.register_filter(Jekyll::Filters)
