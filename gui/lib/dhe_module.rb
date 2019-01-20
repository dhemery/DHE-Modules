require 'color'

require_relative 'controls/button'
require_relative 'controls/counter'
require_relative 'controls/knob'
require_relative 'controls/port'
require_relative 'controls/toggle'
require_relative 'shapes/label'
require_relative 'svg_file'

class DheModule
  HEIGHT = 128.5
  HEIGHT_PX = HEIGHT * PX_PER_MM
  PANEL_LABEL_INSET = 9.0

  attr_reader :source_file, :name, :slug, :width, :foreground, :background, :controls

  def initialize(source_file:, name:, width:, foreground:, background:, controls:, faceplate:)
    @source_file = source_file
    @name = name
    @slug = Pathname(@name.downcase.sub(' ', '-'))
    @width = width
    @width_px = width * PX_PER_MM

    @foreground = foreground
    @background = background
    @faceplate_items = [
        Label.new(x: @width / 2, y: PANEL_LABEL_INSET,
                  text: @name, size: :title, color: @foreground),
        Label.new(x: @width / 2, y: HEIGHT - PANEL_LABEL_INSET,
                  text: 'DHE', size: :title, color: @foreground, alignment: :below)
    ] + faceplate
    @faceplate_items.each do |item|
      item.owner = self
    end
    @controls = controls
    @controls.each do |control|
      control.owner = self
    end
  end

  def faceplate_file
    SvgFile.new(path: slug, width: @width_px, height: HEIGHT_PX, has_text: true) do |svg|
      svg.g(transform: "scale(#{PX_PER_MM})") do |g|
        g.rect(x: 0, y: 0, width: @width, height: HEIGHT,
               stroke: @foreground, fill: @background, 'stroke-width' => 1)
        @faceplate_items.each do |item|
          item.draw_faceplate(svg: g)
        end
      end
    end
  end

  def image_file
    SvgFile.new(path: slug, width: @width_px, height: HEIGHT_PX, has_text: true) do |svg|
      svg.g(transform: "scale(#{PX_PER_MM})") do |g|
        g.rect(x: 0, y: 0, width: @width, height: HEIGHT,
               stroke: @foreground, fill: @background, 'stroke-width' => 1)
        @faceplate_items.each do |item|
          item.draw_faceplate(svg: g)
        end
        @controls.each do |control|
          control.draw_faceplate(svg: g)
        end
      end
    end
  end

  def control_files
    @controls.flat_map(&:svg_files)
  end
end