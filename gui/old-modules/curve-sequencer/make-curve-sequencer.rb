require_relative '../../lib/shapes/position-marker'

module CurveSequencer

  def position_marker(x:, y:, type:)
    marker = PositionMarker.new(type: type, color: @foreground)
    @controls << marker
    @overlay_shapes << marker.translated(x, y)
  end

  def make(steps)
    name "CURVE SEQUENCER #{steps}"
    width_hp = 13.0 + steps * 2.25
    hp width_hp

    hue = 30
    foreground [hue, 100, 10]
    background [hue, 10, 93]

    left = hp2mm(2)
    right = hp2mm(width_hp - 2)
    top = hp2mm(4)
    bottom = hp2mm(23)

    ###############################################################################
    #
    # Sequence Controls (Left)
    #
    ###############################################################################

    sequence_controls_top = top + hp2mm(2.75)
    sequence_controls_bottom = bottom - Port::DIAMETER / 2.0 - 1.0
    sequence_controls_dy = (sequence_controls_bottom - sequence_controls_top) / 4
    run_y = sequence_controls_top + 0 * sequence_controls_dy
    loop_y = sequence_controls_top + 1 * sequence_controls_dy
    selection_y = sequence_controls_top + 2 * sequence_controls_dy
    gate_y = sequence_controls_top + 3 * sequence_controls_dy
    reset_y = sequence_controls_top + 4 * sequence_controls_dy

    input_button_port x: left, y: run_y, label: 'RUN'

    input_button_port x: left, y: gate_y, label: 'GATE'

    selection_length_x = left + hp2mm(2)
    connector left: left, right: selection_length_x, y: selection_y
    small_knob x: left, y: selection_y, label: 'START'
    small_knob x: selection_length_x, y: selection_y, label: 'LENGTH'

    input_button_port x: left, y: loop_y, label: 'LOOP'

    input_button_port x: left, y: reset_y, label: 'RESET'


    ###############################################################################
    #
    # Step Controls
    #
    ###############################################################################

    step_x = hp2mm(10)
    step_dx = hp2mm(2.25)

    active_y = top + Light::RADIUS
    generator_y = top + hp2mm(1.61)
    gate_mode_y = top + hp2mm(3.25)
    level_y = top + hp2mm(5.75)
    shape_y = top + hp2mm(8.75)
    curve_y = top + hp2mm(11.0)
    duration_y = top + hp2mm(14.0)
    enabled_port_y = bottom - Port::DIAMETER / 2.0
    enabled_button_y = enabled_port_y - Port::DIAMETER / 2.0 - Button::DIAMETER / 2.0 - 1.0

    label_x = step_x - 0.6 * step_dx
    label x: label_x, y: generator_y, text: 'GENERATE', alignment: :left_of, size: :large
    label x: label_x, y: gate_mode_y, text: 'ADVANCE', alignment: :left_of, size: :large
    label x: label_x, y: level_y, text: 'LEVEL', alignment: :left_of, size: :large
    label x: label_x, y: shape_y, text: 'SHAPE', alignment: :left_of, size: :large
    label x: label_x, y: curve_y, text: 'CURVE', alignment: :left_of, size: :large
    label x: label_x, y: duration_y, text: 'DURATION', alignment: :left_of, size: :large
    label x: label_x, y: (enabled_button_y + enabled_port_y) / 2, text: 'ENABLED', alignment: :left_of, size: :large

    channel_separator_top = top + hp2mm(1.25)

    line_x = step_x - step_dx / 2.0
    line x1: line_x, x2: line_x, y1: channel_separator_top, y2: bottom

    step_label_y = top - hp2mm(0.5)
    (0...steps).each do |step|
      x = step_x + step * step_dx
      light x: x, y: active_y
      pick_list x: x, y: generator_y, name: 'generate-mode', options: %w[CURVE HOLD SUST INPUT CHASE LEVEL], selection: 1, width: 9
      pick_list x: x, y: gate_mode_y, name: 'advance-mode', options: %w[TIME RISE FALL EDGE HIGH LOW], selection: 1, width: 9
      label x: x, y: step_label_y, text: (step + 1).to_s, alignment: :above, size: :large
      shape_toggle x: x, y: shape_y
      small_knob y: curve_y, x: x, label: ''
      small_knob y: level_y, x: x, label: ''
      small_knob y: duration_y, x: x, label: ''
      button y: enabled_button_y, x: x, label: ''
      port y: enabled_port_y, x: x, label: ''
      line x1: x, x2: x, y1: enabled_button_y, y2: enabled_port_y
      line_x = x + step_dx / 2.0
      line x1: line_x, x2: line_x, y1: channel_separator_top, y2: bottom
    end

    position_marker(x: step_x - Light::DIAMETER, y: active_y, type: :start)
    position_marker(x: step_x + step_dx * (steps - 1) + Light::DIAMETER, y: active_y, type: :end)

    ###############################################################################
    #
    # Sequence Controls (Right)
    #
    ###############################################################################

    out_y = bottom - Port::DIAMETER / 2.0 - 1.0
    in_y = top + hp2mm(2.75)

    input_port x: right, y: in_y, label: 'IN'

    connector left: line_x, right: right, y: level_y
    polarity_toggle x: right, y: level_y, selection: 2

    connector left: line_x, right: right, y: duration_y
    duration_toggle x: right, y: duration_y

    output_port x: right, y: out_y, label: 'OUT'
  end
end
