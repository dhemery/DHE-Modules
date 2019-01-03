require_relative 'shape'

module DHE
  class Label < Shape
    BASELINES = { above: 'alphabetic', below: 'hanging', right_of: 'middle'
    }
    ANCHORS = { above: 'middle', below: 'middle', right_of: 'start'
    }
    ASCENT_RATIO = 2.0 / 3.0 # Approximately correct for Proxima Nova font

    SIZES = { title: 12.0 / PX_PER_MM, large: 9.0 / PX_PER_MM, small: 7.0 / PX_PER_MM
    }

    def initialize(faceplate:, text:, size:, x:, y:, style: :normal, alignment: :above)
      @x = x
      @y = y
      @text = text&.upcase || ''
      @size = SIZES[size.to_sym]
      @color = style == :normal ? faceplate.foreground : faceplate.background
      @alignment = alignment
      @baseline = BASELINES[@alignment]
      @anchor = ANCHORS[@alignment]
      height = @size * ASCENT_RATIO
      width = @text.length * @size * 0.6 # Approximate
      left = case alignment
               when :right_of
                 x + PADDING / 2
               else
                 x - width / 2
             end
      top = case alignment
              when :above
                y - (height + PADDING)
              when :right_of
                y - height / 2
              else
                y + PADDING
            end
      bottom = top + height
      right = left + width
      super(faceplate: faceplate, top: top, right: right, bottom: bottom, left: left)
    end

    def draw(svg:, x: @x, y: @y)
      svg.text(@text, x: x, y: y, 'dominant-baseline' => @baseline, 'text-anchor' => @anchor, fill: @color, style: "font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px")
    end
  end
end