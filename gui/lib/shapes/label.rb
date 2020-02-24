require_relative 'shape'
require_relative '../dimensions'

class Label < Shape
  STYLE_FOR_SIZE = 'font-family:Proxima Nova;font-weight:bold;font-size:%spx'
  ALIGNMENT_TYPES = {
    above: {
      dominant_baseline: 'alphabetic',
      text_anchor: 'middle',
      portion_below: 0.0,
      portion_right: 0.5,
      baseline_shift: 0.0
    },
    center: {
      dominant_baseline: 'middle',
      text_anchor: 'middle',
      portion_below: 0.5,
      portion_right: 0.5,
      baseline_shift: 0.18,
    },
    left_of: {
      dominant_baseline: 'middle',
      text_anchor: 'end',
      portion_below: 0.5,
      portion_right: 0.0,
      baseline_shift: 0.18,
    },
    right_of: {
      dominant_baseline: 'middle',
      text_anchor: 'start',
      portion_below: 0.5,
      portion_right: 1.0,
      baseline_shift: 0.18,
    },
    below: {
      dominant_baseline: 'hanging',
      text_anchor: 'middle',
      portion_below: 1.0,
      portion_right: 0.5,
      baseline_shift: 0.07,
    }
  }
  ASCENT_RATIO = 2.0 / 3.0 # Approximately correct for Proxima Nova font
  SIZES = { title: 12.0 / PX_PER_MM, large: 9.0 / PX_PER_MM, small: 7.0 / PX_PER_MM }

  def initialize(text:, size:, color:, alignment: :above, width: 0.1)
    @text = text
    @text_attributes = {}
    font_size = SIZES[size.to_sym]
    @text_attributes[:style] = STYLE_FOR_SIZE % font_size
    alignment_type = ALIGNMENT_TYPES[alignment]
    @text_attributes['dominant-baseline'] = alignment_type[:dominant_baseline]
    @text_attributes['text-anchor'] = alignment_type[:text_anchor]
    @text_attributes[:fill] = color
    height = font_size * ASCENT_RATIO
    @text_attributes[:y] = height * alignment_type[:baseline_shift]
    right = width * alignment_type[:portion_right]
    bottom = height * alignment_type[:portion_below]
    top = bottom - height
    left = right - width
    super(top: top, right: right, bottom: bottom, left: left)
  end

  def draw(canvas)
    # canvas.line(x1: -5, x2: 5, y1: top, y2: top, stroke: @text_attributes[:fill], 'stroke-width' => '0.01mm')
    # canvas.line(x1: -3.5, x2: 3.5, stroke: @text_attributes[:fill], 'stroke-width' => '0.01mm')
    # canvas.line(x1: -5, x2: 5, y1: bottom, y2: bottom, stroke: @text_attributes[:fill], 'stroke-width' => '0.01mm')
    canvas.text(@text_attributes) do |text|
      text << @text
    end
  end

  def has_text?
    true
  end
end