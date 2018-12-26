module DHE
  class CounterControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_texts = options[:labels]
      @slug = "counter-#{@name}"
      @invisible = options.fetch(:invisible, false)
      @selection = options.fetch(:selection, 1)
      @labels = label_texts.map { |text| Label.new(module_: module_, text: text, size: :small) }
      @button = Button.new(module_: module_)
    end

    def draw_faceplate(svg:, x:, y:) # Counters are not drawn on the faceplate
    end

    def draw_hardware(svg:, x:, y:)
      @labels[@selection - 1].draw_svg(svg: svg, x: x, y: @button.top(y - PADDING)) unless @invisible
      @button.draw_svg(svg: svg, x: x, y: y)
    end

    def path(position)
      module_.slug / "#{@slug}-#{position}"
    end

    def control_file(label:, position:)
      path = @module.slug / "counter-#{@name}-#{position}"
      width = @button.width
      height = @button.height + (PADDING + label.height) * 2
      x = width / 2
      y = height / 2
      SvgFile.new(path: path, width: width, height: height, has_text: true) do |svg|
        @button.draw_svg(svg: svg, x: x, y: y)
        label.draw_svg(svg: svg, x: x, y: @button.top(y - PADDING))
      end
    end

    def control_files
      @labels.each_with_index.map { |label, index| control_file(label: label, position: index + 1) }
    end
  end
end