module DHE
  class Control
    attr_reader :name, :row, :column

    def initialize(spec:)
      @name = spec[:name]
      @row = spec[:row]
      @column = spec[:column]
    end

    def to_s
      "[#{@column},#{@row}]#{' ' + @name if @name}"
    end

    def draw_on_image(panel:, svg:)
    end
  end
end
