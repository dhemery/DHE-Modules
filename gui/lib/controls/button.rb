require_relative '../shapes/circle'

class Button
  DIAMETER = 6.0

  attr_reader :frames, :pressed, :released

  def initialize(name: 'button', pressed_color:, released_color:)
    @pressed = Frame.new(slug: "#{name}-pressed", button_color: pressed_color, ring_color: released_color)
    @released = Frame.new(slug: "#{name}-released", button_color: released_color, ring_color: released_color)
    @frames = [pressed, released]
  end

  class Frame < Shape
    RADIUS = DIAMETER / 2.0
    RING_RADIUS = DIAMETER / 2.0
    RING_THICKNESS = DIAMETER / 6.0
    BUTTON_RADIUS = RING_RADIUS - RING_THICKNESS

    attr_reader :slug

    def initialize(slug:, button_color:, ring_color:)
      super(top: -RADIUS, right: RADIUS, bottom: RADIUS, left: -RADIUS)
      @slug = Pathname(slug)

      @shapes = [
        Circle.new(radius: RING_RADIUS, fill: ring_color, stroke: :none, stroke_width: 0),
        Circle.new(radius: BUTTON_RADIUS, fill: button_color, stroke: :none, stroke_width: 0),
      ]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end
  end
end

