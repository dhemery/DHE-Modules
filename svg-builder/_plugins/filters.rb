module Jekyll
  module Filters
    PX_PER_MM = 2.9527559055
    def mm_to_px(mm)
      mm * PX_PER_MM
    end
    module_function :mm_to_px

    PORT_DIAMETER = mm_to_px(8.4)
    PORT_RADIUS = PORT_DIAMETER / 2.0
    BUTTON_DIAMETER = mm_to_px(6.0)
    BUTTON_RADIUS = BUTTON_DIAMETER / 2.0
    SWITCH_WIDTH = mm_to_px(3.0)
    SWITCH_2_HEIGHT = mm_to_px(6.0)
    SWITCH_3_HEIGHT = mm_to_px(9.0)
    LARGE_KNOB_DIAMETER = mm_to_px(13.0)
    SMALL_LABEL_FONT_SIZE = 7.0 # px
    LARGE_LABEL_FONT_SIZE = 9.0 # px

    class RoundControl
      def initialize(x, y, diameter)
        @x = x
        @y = y
        @diameter = diameter
        @radius = diameter / 2.0
      end

      def top
        @y - @radius
      end

      def left
        @x - @radius
      end

      def right
        @x + @radius
      end

      def width
        @diameter
      end

      def bottom
        @y + @radius
      end
    end

    class Label
      ASCENT_RATIO = 0.66666666666 # For Proxima Nova font

      def initialize(x, y, text, color, size, alignment = :above)
        @x = x
        @y = y
        @text = text.upcase
        @color = color
        @size = size
        @height = @size * ASCENT_RATIO
        @alignment = alignment
        case @alignment
        when :above
          @baseline = "baseline"
          @anchor = "middle"
        when :below
          @baseline = "hanging"
          @anchor = "middle"
        when :right
          @baseline = "middle"
          @anchor = "start"
        end
      end

      def top
        case @alignment
        when :above
          @y - @height
        when :below
          @y
        when :right
          @y - @height / 2.0
        end
      end

      def to_liquid
        %Q[<text dominant-baseline="#{@baseline}" text-anchor="#{@anchor}" fill="#{@color}" x="#{@x}" y="#{@y}" style="font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px">#{@text}</text>]
      end
    end

    class Box
      STROKE_WIDTH = Filters::mm_to_px(0.5)
      PADDING = Filters::mm_to_px(1.0)
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
        port = RoundControl.new(x, y, PORT_DIAMETER)
        @label = Label.new(x, port.top - Box::PADDING, label_text, label_color, SMALL_LABEL_FONT_SIZE)
        @box = Box.new(@label.top, port.right, port.bottom, port.left, border, background)
      end

      def to_liquid
        @box.to_liquid + @label.to_liquid
      end
    end

    class PortButtonBox
      PORT_BUTTON_DISTANCE = PORT_RADIUS + BUTTON_RADIUS + Box::PADDING
      def initialize(x, y, direction, border, background, label_text, label_color)
        button_x = direction == :right ? x + PORT_BUTTON_DISTANCE : x - PORT_BUTTON_DISTANCE
        port = RoundControl.new(x, y, PORT_DIAMETER)
        button = RoundControl.new(button_x, y, BUTTON_DIAMETER)
        right = direction == :right ? button.right : port.right
        left = direction == :left ? button.left : port.left
        @label = Label.new(x, port.top - Box::PADDING, label_text, label_color, SMALL_LABEL_FONT_SIZE)
        @box = Box.new(@label.top, right, port.bottom, left, border, background)
      end

      def to_liquid
        @box.to_liquid + @label.to_liquid
      end
    end

    class LabeledRoundControl
      def initialize(x, y, diameter, text, color, font_size)
        control = RoundControl.new(x, y, diameter)
        @label = Label.new(x, control.top - Box::PADDING, text, color, font_size)
      end

      def to_liquid
        @label.to_liquid
      end
    end

    class LabeledRectangularControl
      def initialize(x, y, width, height, color, top_label, bottom_label, right_label)
        top = y - height / 2.0
        bottom = y + height / 2.0
        left = x - width / 2.0
        right = x + width / 2.0
        @labels = [
          Label.new(x, top - Box::PADDING, top_label, color, SMALL_LABEL_FONT_SIZE, :above),
          Label.new(x, bottom + Box::PADDING, bottom_label, color, SMALL_LABEL_FONT_SIZE, :below)
        ]
        @labels << Label.new(right + Box::PADDING / 2.0, y, right_label, color, SMALL_LABEL_FONT_SIZE, :right) if right_label
      end

      def to_liquid
        @labels.map(&:to_liquid).join
      end
    end

    def labeled_round_control(x_mm, y_mm, diameter, label, color, font_size)
      x_px = Filters::mm_to_px(x_mm)
      y_px = Filters::mm_to_px(y_mm)
      LabeledRoundControl.new(x_px, y_px, diameter, label, color, font_size)
    end

    def port_box(x_mm, y_mm, border, background, label, label_color)
      x_px = Filters::mm_to_px(x_mm)
      y_px = Filters::mm_to_px(y_mm)
      PortBox.new(x_px, y_px, border, background, label, label_color)
    end

    def port_button_box(x_mm, y_mm, direction, border, background, label, label_color)
      x_px = Filters::mm_to_px(x_mm)
      y_px = Filters::mm_to_px(y_mm)
      PortButtonBox.new(x_px, y_px, direction, border, background, label, label_color)
    end

    def cv_port_box(page, x_mm, y_mm)
      port_box(x_mm, y_mm, "none", "none", "CV", page["dark_color"])
    end

    def in_port_button_box(page, x_mm, y_mm, label)
      port_button_box(x_mm, y_mm, :right, page["dark_color"], page["light_color"], label, page["dark_color"])
    end

    def out_port_button_box(page, x_mm, y_mm, label)
      port_button_box(x_mm, y_mm, :left, page["dark_color"], page["dark_color"], label, page["light_color"])
    end

    def in_port_box(page, x_mm, y_mm, label)
      port_box(x_mm, y_mm, page["dark_color"], page["light_color"], label, page["dark_color"])
    end

    def out_port_box(page, x_mm, y_mm, label)
      port_box(x_mm, y_mm, page["dark_color"], page["dark_color"], label, page["light_color"])
    end

    def labeled_large_knob(page, x_mm, y_mm, label)
      labeled_round_control(x_mm, y_mm, LARGE_KNOB_DIAMETER, label, page['dark_color'], LARGE_LABEL_FONT_SIZE)
    end

    def labeled_switch(page, x_mm, y_mm, top_label, bottom_label, right_label = nil)
      height = right_label ? SWITCH_3_HEIGHT : SWITCH_2_HEIGHT
      x_px = Filters::mm_to_px(x_mm)
      y_px = Filters::mm_to_px(y_mm)
      LabeledRectangularControl.new(x_px, y_px, SWITCH_WIDTH, height, page['dark_color'], top_label, bottom_label, right_label)
    end
  end
end

Liquid::Template.register_filter(Jekyll::Filters)
