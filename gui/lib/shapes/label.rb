require_relative 'shape'
require_relative '../dimensions'

class Label < BoundedShape
  STYLE_FOR_SIZE = 'font-family:Proxima Nova;font-weight:bold;font-size:%spx'
  BASELINES = { above: 'alphabetic', below: 'hanging', right_of: 'middle' }
  ANCHORS = { above: 'middle', below: 'middle', right_of: 'start' }
  ASCENT_RATIO = 2.0 / 3.0 # Approximately correct for Proxima Nova font
  SIZES = { title: 12.0 / PX_PER_MM, large: 9.0 / PX_PER_MM, small: 7.0 / PX_PER_MM }

  def initialize(text:, size:, color:, alignment: :above)
    @text = text
    @size = SIZES[size.to_sym]
    @color = color
    @alignment = alignment
    @baseline = BASELINES[@alignment]
    @anchor = ANCHORS[@alignment]
    height = @size * ASCENT_RATIO
    width = 0.1 # Ignore the actual width of the label
    left = case alignment
             when :right_of
               0.0
             else # above or below
               -width / 2.0
           end
    top = case alignment
            when :above
              -height
            when :right_of
              -height / 2.0
            else # below
              0.0
          end
    bottom = top + height
    right = left + width
    super(top: top, right: right, bottom: bottom, left: left)
  end

  def draw(canvas)
    canvas.text('dominant-baseline' => @baseline, 'text-anchor' => @anchor, fill: @color, style: STYLE_FOR_SIZE % @size) do |text|
      text << @text
    end
  end
end