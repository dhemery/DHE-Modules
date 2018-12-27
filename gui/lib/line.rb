require 'ostruct'
require_relative 'shape'

module DHE
  class Line
    attr_reader :start, :end
    def initialize(module_:, options:)
      @stroke = module_.foreground
      @start = OpenStruct.new(options[:start])
      @start.x_offset = 0
      @start.y_offset = 0
      @end = OpenStruct.new(options[:end])
      @end.x_offset = 0
      @end.y_offset = 0
    end

    def draw_svg(svg:, x1:, y1:, x2:, y2:)
      svg.line(x1: x1, y1: y1, x2: x2, y2: y2, 'stroke-width' => STROKE_WIDTH, stroke: @stroke)
    end
  end
end