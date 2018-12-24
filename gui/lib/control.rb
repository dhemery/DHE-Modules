module DHE
  class Control
    attr_reader :name, :row, :column

    def initialize(spec:)
      @name = spec[:name]
      @row = spec[:row]
      @column = spec[:column]
    end
  end

  class RoundControl < Control
    attr_reader :diameter

    def initialize(spec:, diameter:)
      super(spec: spec)
      @diameter = diameter
    end

    def radius
      diameter / 2
    end
  end
end
