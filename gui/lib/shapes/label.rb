require_relative 'shape'
require_relative '../dimensions'

class Label < BoundedShape
  STYLE_FOR_SIZE = 'font-family:Proxima Nova;font-weight:bold;font-size:%spx'
  BASELINES = { center: 'middle', above: 'alphabetic', below: 'hanging', right_of: 'middle', left_of: 'middle' }
  ANCHORS = { center: 'middle', above: 'middle', below: 'middle', right_of: 'start', left_of: 'end' }
  ASCENT_RATIO = 2.0 / 3.0 # Approximately correct for Proxima Nova font
  SIZES = { title: 12.0 / PX_PER_MM, large: 9.0 / PX_PER_MM, small: 7.0 / PX_PER_MM }

  def initialize(text:, size:, color:, alignment: :above)
    @text = text
    @text_attributes = {}
    @text_attributes['dominant-baseline'] = BASELINES[alignment]
    @text_attributes['text-anchor'] = ANCHORS[alignment]
    @text_attributes[:fill] = color
    font_size = SIZES[size.to_sym]
    @text_attributes[:style] = STYLE_FOR_SIZE % font_size
    height = font_size * ASCENT_RATIO
    width = 0.1 # Ignore the actual width of the label
    left = case alignment
           when :left_of, :right_of
             0.0
           else # center, above, or below
             -width / 2.0
           end
    baseline = -height / 5.0
    top = case alignment
          when :above
            baseline - height
          when :left_of, :right_of, :center
            baseline - height / 2.0
          else # below
            baseline
          end
    bottom = top + height
    right = left + width
    super(top: top, right: right, bottom: bottom, left: left)
  end

  def draw(canvas)
    canvas.text(@text_attributes) do |text|
      text << @text
    end
  end

  def has_text?
    true
  end
end