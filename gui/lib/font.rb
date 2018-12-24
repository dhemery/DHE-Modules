require_relative 'dimensions'

module DHE
  class Font
    def initialize(size)
      @size = size / PX_PER_MM
    end

    def text_style
      "font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px"
    end

    PANEL = Font.new(12.0)
    LARGE = Font.new(9.0)
    SMALL = Font.new(7.0)
  end
end