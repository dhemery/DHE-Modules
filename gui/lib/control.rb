require 'color'
require 'oj'


module DHE
  class Control
    attr_reader :x, :y

    def initialize(mod:, name:, type:, x:, y:)
      @mod = mod
      @name = name
      @type = type
      @x = x
      @y = y
    end

    def self.from(mod:, details:)
      name = details[:name]
      type = details[:type]
      x = details[:x]
      y = details[:y]
      Control.new(mod: mod, name: name, type: type, x: x, y: y)
    end

    def to_s
      "#{@name}: #{@type} {#{@x},#{@y}}"
    end
  end

  class Knob < Control
  end

  class Button < Control
  end

  class Port < Control
  end

  class Toggle < Control
  end
end
