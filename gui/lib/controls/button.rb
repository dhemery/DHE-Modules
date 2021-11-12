require_relative '../shapes/circle'

class Button
  DIAMETER = 6.0

  attr_reader :pressed, :released

  def initialize(name: 'button', pressed_color:, released_color:)
    @pressed = State.new(name: name, state: :pressed, button_color: pressed_color, ring_color: released_color)
    @released = State.new(name: name, state: :released, button_color: released_color, ring_color: released_color)
  end

  def frames
    [pressed, released]
  end

  class State < Shape
    RADIUS = DIAMETER / 2.0
    RING_RADIUS = DIAMETER / 2.0
    RING_THICKNESS = DIAMETER / 6.0
    BUTTON_RADIUS = RING_RADIUS - RING_THICKNESS

    attr_reader :slug

    def initialize(name:, state:, button_color:, ring_color:)
      super(top: -RADIUS, right: RADIUS, bottom: RADIUS, left: -RADIUS)
      @slug = Pathname("#{name}-#{state}")

      @shapes = [
        Circle.new(radius: RING_RADIUS, fill: ring_color, stroke: :none, stroke_width: 0),
        Circle.new(radius: BUTTON_RADIUS, fill: button_color, stroke: :none, stroke_width: 0),
      ]
    end

    def draw(canvas)
      @shapes.each { |state| state.draw(canvas) }
    end
  end
end

