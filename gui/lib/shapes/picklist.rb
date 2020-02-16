require_relative 'shape'
require_relative 'label'
require_relative '../dimensions'

module Picklist
  BOX_STROKE_WIDTH = 0.25
  BOX_CORNER_RADIUS = BOX_STROKE_WIDTH * 2

  class Option < CenteredShape
    attr_reader :slug

    def initialize(name:, text:, position:, foreground:, background:, width:)
      @background = background
      @slug = Pathname("picklist-#{name}-#{position}")
      @label = Label.new(color: foreground, alignment: :center, size: :small, text: text)
      super(width: width, height: @label.height + 2 * PADDING)
    end

    def draw(canvas)
      canvas.rect(x: left, width: width, y: top, height: height,
                  rx: BOX_CORNER_RADIUS, ry: BOX_CORNER_RADIUS,
                  stroke: 'none', fill: @background)
      @label.draw(canvas)
    end

    def has_text?
      true
    end
  end

  class Menu < CenteredShape
    attr_reader :slug

    def initialize(name:, options:, foreground:, background:, width:)
      labels = options.map { |option| Label.new(color: foreground, alignment: :center, size: :small, text: option) }
      label_height = labels[0].height + 2 * PADDING
      super(width: width + BOX_STROKE_WIDTH, height: labels.size * label_height + BOX_STROKE_WIDTH)

      @stroke = foreground
      @fill = background
      @slug = Pathname("picklist-#{name}")
      label_origin_y = 0 - (height - label_height - BOX_STROKE_WIDTH) / 2
      @labels = labels.each_with_index.map { |label, index| label.translate(0, label_origin_y + label_height * index) }
      @labels = labels.each_with_index.map { |label, index| label.translate(0, label_origin_y + label_height * index) }
    end

    def draw(canvas)
      canvas.rect(x: left, width: width, y: top, height: height,
                  rx: BOX_CORNER_RADIUS, ry: BOX_CORNER_RADIUS,
                  'stroke-width' => BOX_STROKE_WIDTH,
                  stroke: @stroke, fill: @fill)
      @labels.each do |label|
        canvas.rect(x: (BOX_STROKE_WIDTH - width) / 2, y: label.top - PADDING,
                    width: width, height: label.height + 2 * PADDING,
                    rx: BOX_CORNER_RADIUS, ry: BOX_CORNER_RADIUS,
                    'stroke-width' => BOX_STROKE_WIDTH,
                    stroke: @stroke, fill: @fill)
        label.draw(canvas)
      end
    end

    def has_text?
      true
    end
  end
end
