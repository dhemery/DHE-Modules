module DHE
  class Control
    attr_reader :x, :y

    def initialize(spec:)
      @x = spec[:x]
      @y = spec[:y]
      @name = spec[:name]
    end

    def to_s
      "[#{@x},#{@y}]#{' ' + @name if @name}"
    end
  end
end
