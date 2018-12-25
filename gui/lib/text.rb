require_relative 'control'

module DHE

  class Text < Bounded
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

    def initialize(text:, size:, alignment:, padding: PADDING)
      @text = text&.upcase || ''
      @size = SIZES[size.to_sym]
      super(width: @text.length * @size * 0.6, height: @size)
      @alignment = alignment
      @baseline = BASELINES[@alignment]
      @anchor = ANCHORS[@alignment]
      @padding = padding
    end

    def draw_svg(svg:, x:, y:, color:)
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
               'dominant-baseline' => @baseline, 'text-anchor' => @anchor, fill: color,
               style: "font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px")
    end
  end
end
