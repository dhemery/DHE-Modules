module DHE
  class CounterControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_texts = options[:labels]
      @invisible = options.fetch(:invisible, false)
      @selection = options.fetch(:selection, 1)
      @labels = label_texts.map {|text| Label.new(module_: module_, text: text, size: :small)}
      @button = Button.new(module_: module_)
    end

    def draw_faceplate(svg:, x:, y:)
    end

    def draw_hardware(svg:, x:, y:)
      @labels[@selection - 1].draw_svg(svg: svg, x: x, y: @button.top(y - PADDING)) unless @invisible
      @button.draw_svg(svg: svg, x: x, y: y)
    end

    def control_file(label:, position:)
      path = @module.slug / "counter-#{@name}-#{position}"
      width = @button.width
      height = @button.height + (PADDING + label.height) * 2
      x = width / 2
      y = height / 2
      content = Builder::XmlMarkup.new(indent: 2)
                    .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                         width: width,
                         height: height) do |svg|
        @button.draw_svg(svg: svg, x: x, y: y)
        label.draw_svg(svg: svg, x: x, y: @button.top(y - PADDING))
      end
      SvgFile.new(path: path, content: content, has_text: true)
    end

    def control_files
      @labels.each_with_index.map {|label, i| control_file(label: label, position: i + 1)}
    end
  end
end