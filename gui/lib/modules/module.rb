require 'builder'
require 'color'
require 'oj'

require_relative '../shapes/dimensions'
require_relative '../shapes/knob'
require_relative '../shapes/label'
require_relative '../shapes/line'
require_relative '../shapes/port'
require_relative '../shapes/toggle'
require_relative '../svg_file'

module DHE
  JSON_PARSING_OPTIONS = { symbol_keys: true }

  class Module
    attr_reader :name, :slug, :foreground, :background

    def initialize(name:, hp:, foreground:, background:)
      @name = name.upcase
      @slug = Pathname(@name.downcase.sub(' ', '-'))
      @width = hp * MM_PER_HP
      @width_px = @width * PX_PER_MM
      @height_px = PANEL_HEIGHT * PX_PER_MM

      @foreground = "##{Color::HSL.new(*foreground).to_rgb.hex}"
      @background = "##{Color::HSL.new(*background).to_rgb.hex}"
      @faceplate_items = [
          Label.new(faceplate: self, text: @name, size: :title, x: @width / 2, y: PANEL_LABEL_INSET),
          Label.new(faceplate: self, text: 'DHE', size: :title, alignment: :below, x: @width / 2, y: PANEL_HEIGHT - PANEL_LABEL_INSET)
      ]
      @controls = []
    end

    def faceplate_file
      SvgFile.new(path: slug, width: @width_px, height: @height_px, has_text: true) do |svg|
        svg.g(transform: "scale(#{PX_PER_MM})") do |g|
          g.rect(x: 0, y: 0, width: @width, height: PANEL_HEIGHT,
                 stroke: @foreground, fill: @background, 'stroke-width' => 1)
          @faceplate_items.each do |item|
            item.draw(svg: g)
          end
        end
      end
    end

    def manual_image_file
      SvgFile.new(path: slug, width: @width_px, height: @height_px, has_text: true) do |svg|
        svg.g(transform: "scale(#{PX_PER_MM})") do |g|
          g.rect(x: 0, y: 0, width: @width, height: PANEL_HEIGHT,
                 stroke: @foreground, fill: @background, 'stroke-width' => 1)
          @faceplate_items.each do |item|
            item.draw(svg: g)
          end
          @controls.each do |control|
            control.draw(svg: g)
          end
        end
      end
    end

    def control_files
      []
    end

    def connector(x1:, y1:, x2:, y2:)
      @faceplate_items << Line.new(faceplate: self, x1: x1, y1: y1, x2: x2, y2: y2)
    end

    def cv_port(x:, y:)
      port = Port.new(faceplate: self, x: x, y: y)
      label = Label.new(faceplate: self, text: 'cv', size: :small, x: x, y: port.top - PADDING)
      @faceplate_items << label
      @controls << port
    end

    def input_port(x:, y:, label: 'in')
      port = Port.new(faceplate: self, x: x, y: y)
      label = Label.new(faceplate: self, text: label, size: :small, x: x, y: port.top - PADDING)
      @faceplate_items << Box.new(faceplate: self, top: label.top, right: port.right, bottom: port.bottom, left: port.left)
      @faceplate_items << label
      @controls << port
    end

    def output_port(x:, y:, label: 'out')
      port = Port.new(faceplate: self, x: x, y: y)
      label = Label.new(faceplate: self, text: label, size: :small, x: x, y: port.top - PADDING, style: :reversed)
      @faceplate_items << Box.new(faceplate: self, style: :reversed, top: label.top, right: port.right, bottom: port.bottom, left: port.left)
      @faceplate_items << label
      @controls << port
    end

    def large_knob(x:, y:, label:)
      knob = Knob.new(faceplate: self, size: :large, x: x, y: y)
      @controls << knob
      @faceplate_items << Label.new(faceplate: self, text: label, size: :large, x: x, y: knob.top - PADDING)
    end

    def toggle(x:, y:, labels:, position: 1)
      toggle = Toggle.new(faceplate: self, x: x, y: y, size: labels.size, position: position)
      @controls << toggle
      @faceplate_items << Label.new(faceplate: self, text: labels.first, size: :small, x: x, y: toggle.bottom + PADDING,
                                    alignment: :below)
      @faceplate_items << Label.new(faceplate: self, text: labels[1], size: :small, x: toggle.right + PADDING / 2.0,
                                    y: y,
                                    alignment: :right_of) if labels.size == 3
      @faceplate_items << Label.new(faceplate: self, text: labels.last, size: :small, x: x, y: toggle.top - PADDING,
                                    alignment:
                                        :above)
    end

    def polarity_switch(x:, y:)
      toggle(x: x, y: y, labels: %w(bi uni))
    end

    def shape_switch(x:, y:)
      toggle(x: x, y: y, labels: %w(j s))
    end

    def duration_switch(x:, y:)
      toggle(x: x, y: y, labels: %w(1 10 100), position: 2)
    end
  end
end