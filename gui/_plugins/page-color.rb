module DHE
  module PageColor
    def rgbhex(color)
      "##{color.to_rgb.hex}"
    end

    def background(page)
      rgbhex(Color::HSL.new(*page['background-color']))
    end

    def foreground(page)
      rgbhex(Color::HSL.new(*page['foreground-color']))
    end
  end
end