class Shape
  attr_reader :x, :y, :top, :right, :bottom, :left, :width, :height

  def initialize(x: 0.0, y: 0.0, top:, right:, bottom:, left:)
    @x = x
    @y = y
    @top = top
    @right = right
    @bottom = bottom
    @left = left
    @width = right - left
    @height = bottom - top
  end

  def translated(dx, dy)
    Translated.new(shape: self, dx: dx, dy: dy)
  end

  def padded(all: 0.0, v: all, h: all, top: v, bottom: v, right: h, left: h)
    Padded.new(shape: self, top: top, right: right, bottom: bottom, left: left)
  end

  def has_text?
    false
  end
end

class CompositeShape < Shape
  def initialize(shapes)
    super(top: shapes.map(&:top).min,
          right: shapes.map(&:right).max,
          bottom: shapes.map(&:bottom).max,
          left: shapes.map(&:left).min)
    @shapes = shapes
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end

class Translated < Shape
  def initialize(dx:, dy:, shape:)
    super(
      x: shape.x + dx,
      y: shape.y + dy,
      top: shape.top + dy,
      right: shape.right + dx,
      bottom: shape.bottom + dy,
      left: shape.left + dx
    )
    @dx = dx
    @dy = dy
    @shape = shape
  end

  def draw(canvas)
    canvas.g(transform: "translate(#{@dx},#{@dy})") do |g|
      @shape.draw(g)
    end
  end
end

class Padded < Shape
  def initialize(shape:, top:, bottom:, right:, left:)
    super(
      x: shape.x,
      y: shape.y,
      top: shape.top - top,
      right: shape.right + right,
      bottom: shape.bottom + bottom,
      left: shape.left - left
    )
    @shape = shape
  end

  def draw(canvas)
    @shape.draw(canvas)
  end
end
