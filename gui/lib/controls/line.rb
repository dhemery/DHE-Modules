require 'ostruct'
require_relative '../shape'

class Line < Shape
  attr_reader :start, :end

  def initialize(color:, x1:, y1:, x2:, y2:)
    @stroke = color
    @x1 = x1
    @y1 = y1
    @x2 = x2
    @y2 = y2
  end

  def draw_faceplate(svg:)
    svg.line(x1: @x1, y1: @y1, x2: @x2, y2: @y2, 'stroke-width' => STROKE_WIDTH, stroke: @stroke)
  end
end