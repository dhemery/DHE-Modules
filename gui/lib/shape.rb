class Shape
  attr_reader :width, :height, :top, :right, :bottom, :left, :x, :y
  attr_accessor :owner

  def initialize(top:, right:, bottom:, left:, x: (right + left) / 2.0, y: (bottom + top) / 2.0)
    @top = top
    @right = right
    @bottom = bottom
    @left = left
    @width = right - left
    @height = bottom - top
    @x = x
    @y = y
  end

  def self.centered(x:, y:, width:, height: width)
    { left: x - width / 2, right: x + width / 2, top: y - height / 2, bottom: y + height / 2, }
  end

  def draw_faceplate(svg:)
    draw(svg: svg, x: @x, y: @y)
  end
end
