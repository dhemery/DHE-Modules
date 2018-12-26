require_relative 'shape'

module DHE
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
      svg.text(@text, x: x, y: y,
               'dominant-baseline' => @baseline, 'text-anchor' => @anchor, fill: @color,
               style: "font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px")
    end
  end
end