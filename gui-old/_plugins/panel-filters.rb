require 'ostruct'
require_relative 'page-color'
require_relative 'controls'

module PanelFilters
  include DHE::PageColor

  def port(x:, y:, style: :normal, label:, foreground:, background:, draw:)
    text_color = style == :normal ? foreground : background
    port = PortControl.new(x: x, y: y, foreground: foreground, background: background)
    label_text = Text.new(text: label, color: text_color, size: SMALL_FONT)
    label = Label.new(label_text, PADDING, :above, port)
    box = Box.around(content: [port, label], style: style, foreground: foreground, background: background)
    items = [box, label]
    items << port if draw
    items.map(&:svg).join("\n")
  end

  def port_button(port_x:, port_y:, style: :normal, button_position:, label:, foreground:, background:, draw:)
    text_color = style == :normal ? foreground : background
    port = PortControl.new(x: port_x, y: port_y, foreground: foreground, background: background)
    label_text = Text.new(text: label, color: text_color, size: SMALL_FONT)
    label = Label.new(label_text, PADDING, :above, port)
    button = ButtonControl.new(style: style, foreground: foreground, background: background)
    button.align(PADDING, button_position, port)
    box = Box.around(content: [port, label, button], style: style, foreground: foreground, background: background)
    items = [box, label]
    items += [port, button] if draw
    items.map(&:svg).join("\n")
  end

  def knob(page:, x:, y:, size:, label:, label_size:)
    foreground = foreground(page)
    background = background(page)
    knob = KnobControl.new(x: x, y: y, size: size, foreground: foreground, background: background)
    label_text = Text.new(text: label, color: foreground, size: label_size)
    items = [Label.new(label_text, PADDING, :above, knob)]
    items << knob if page['draw_controls']
    items.map(&:svg).join("\n")
  end

  def button(page, x, y, label)
    background = background(page)
    foreground = foreground(page)
    button = ButtonControl.new(x: x, y: y, foreground: foreground, background: background)
    label_text = Text.new(text: label, color: foreground, size: SMALL_FONT)
    items = [Label.new(label_text, PADDING, :above, button)]
    items << button if page['draw_controls']
    items.map(&:svg).join("\n")
  end

  def toggle_button(page, x, y, label)
    background = background(page)
    foreground = foreground(page)
    button = ButtonControl.new(x: x, y: y, foreground: foreground, background: background)
    text = Text.new(text: label, color: foreground, size: SMALL_FONT)
    items = [
      Label.new(text, PADDING, :above, button),
      button,
    ]
    items.map(&:svg).join("\n") if page['draw_controls']
  end

  def tiny_knob(page, x, y, label, label_size = SMALL_FONT)
    knob(page: page, x: x, y: y, size: :tiny, label: label, label_size: label_size)
  end

  def small_knob(page, x, y, label)
    knob(page: page, x: x, y: y, size: :small, label: label, label_size: SMALL_FONT)
  end

  def medium_knob(page, x, y, label)
    knob(page: page, x: x, y: y, size: :medium, label: label, label_size: LARGE_FONT)
  end

  def large_knob(page, x, y, label)
    knob(page: page, x: x, y: y, size: :large, label: label, label_size: LARGE_FONT)
  end

  def attenuverter(page, x, y)
    knob(page: page, x: x, y: y, size: :tiny, label: '<tspan font-size="larger">-&#160;&#160;+</tspan>', label_size: LARGE_FONT)
  end

  def cv(page, x, y)
    foreground = foreground(page)
    background = background(page)
    draw = page['draw_controls']
    port = PortControl.new(x: x, y: y, foreground: foreground, background: background)
    label_text = Text.new(text: 'CV', color: foreground, size: SMALL_FONT)
    items = [Label.new(label_text, PADDING, :above, port)]
    items << port if draw
    items.map(&:svg).join("\n")
  end


  def in_port_button(page, x, y, label)
    foreground = foreground(page)
    background = background(page)
    draw = page['draw_controls']
    port_button(port_x: x, port_y: y, button_position: :right_of, label: label, foreground: foreground, background: background, draw: draw)
  end

  def out_port_button(page, x, y, label)
    foreground = foreground(page)
    background = background(page)
    draw = page['draw_controls']
    port_button(port_x: x, port_y: y, button_position: :left_of, style: :reverse, label: label, foreground: foreground, background: background, draw: draw)
  end

  def in_port(page, x, y, label)
    foreground = foreground(page)
    background = background(page)
    draw = page ['draw_controls']
    port(x: x, y: y, label: label, foreground: foreground, background: background, draw: draw)
  end

  def out_port(page, x, y, label)
    foreground = foreground(page)
    background = background(page)
    draw = page ['draw_controls']
    port(x: x, y: y, style: :reverse, label: label, foreground: foreground, background: background, draw: draw)
  end

  def duration_switch(page, x, y)
    thumb3(page, x, y, 1, 10, 100, 2)
  end

  def polarity_switch(page, x, y, selected = 2)
    thumb2(page, x, y, 'BI', 'UNI', selected)
  end

  def shape_switch(page, x, y)
    thumb2(page, x, y,  'J', 'S', 1)
  end

  def thumb2(page, x, y, low, high, selected)
    foreground = foreground(page)
    background = background(page)
    draw = page['draw_controls']
    switch = SwitchControl.new(x: x, y: y, size: 2, selection: selected, foreground: foreground, background: background)
    high_text = Text.new(text: high, size: SMALL_FONT, color: foreground)
    low_text = Text.new(text: low, size: SMALL_FONT, color: foreground)
    items = [
        Label.new(high_text, PADDING + STROKE_INSET, :above, switch),
        Label.new(low_text, PADDING + STROKE_INSET, :below, switch),
    ]
    items << switch if draw
    items.map(&:svg).join("\n")
  end

  def thumb3(page, x, y, low, mid, high, selected)
    foreground = foreground(page)
    background = background(page)
    draw = page['draw_controls']
    switch = SwitchControl.new(x: x, y: y, size: 3, selection: selected, foreground: foreground, background: background)
    high_text = Text.new(text: high, size: SMALL_FONT, color: foreground)
    mid_text = Text.new(text: mid, size: SMALL_FONT, color: foreground)
    low_text = Text.new(text: low, size: SMALL_FONT, color: foreground)
    items = [
        Label.new(high_text, PADDING + STROKE_INSET, :above, switch),
        Label.new(mid_text, PADDING / 2.0 + STROKE_INSET, :right_of, switch),
        Label.new(low_text, PADDING + STROKE_INSET, :below, switch)
    ]

    items << switch if draw
    items.map(&:svg).join("\n")
  end

  def panel(page)
    foreground = foreground(page)
    background = background(page)
    panel = Bounded.new(top: 0.0, right: width(page), bottom: PANEL_HEIGHT, left: 0.0)
    name_label_text = Text.new(text: page['title'], color: foreground, size: PLUGIN_FONT)
    name_label = Label.new(name_label_text, -PLUGIN_LABEL_INSET, :above, panel)
    author_label_text = Text.new(text: 'DHE', color: foreground, size: PLUGIN_FONT)
    author_label = Label.new(author_label_text, -PLUGIN_LABEL_INSET, :below, panel)
    box = %Q[<rect x="#{panel.left}" y="#{panel.top}" width="#{panel.width}" height="#{panel.height}" stroke="#{foreground}" fill="#{background}" stroke-width="1"/>]
    box + [name_label, author_label].map(&:svg).join("\n")
  end

  def connector(page, x1, y1, x2, y2)
    %Q[<line x1="#{x1}" y1="#{y1}" x2="#{x2}" y2="#{y2}" stroke="#{foreground(page)}" stroke-width="#{STROKE_WIDTH}" />]
  end

  def width(page)
    hp_to_mm(page['width'])
  end

  def mm_to_px(mm)
    mm * PX_PER_MM
  end

  def hp_to_mm(hp)
    hp * MM_PER_HP
  end
end

Liquid::Template.register_filter(PanelFilters)
