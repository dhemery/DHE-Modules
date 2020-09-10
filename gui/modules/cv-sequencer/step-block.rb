require_relative 'style'

STEPS = 4
STEP_BLOCK_WIDTH_HP = 9.0
STEP_BLOCK_WIDTH_MM = hp2mm(STEP_BLOCK_WIDTH_HP)
STEP_DX_MM = STEP_BLOCK_WIDTH_MM / (STEPS - 1)

KNOB_BUTTON_DISTANCE_MM = hp2mm(1.3)
BUTTON_BUTTON_DISTANCE_MM = hp2mm(0.8)
ADVANCE_ANCHOR_DISTANCE_MM = hp2mm(2.75)
ANCHOR_ANCHOR_DISTANCE_MM = hp2mm(4.25)
ANCHOR_DURATION_DISTANCE = hp2mm(3.25)
DURATION_SHAPE_DISTANCE = hp2mm(3.25)

STEP_TOP_MM = TOP_MM + hp2mm(1.25)
PROGRESS_LIGHT_Y_MM = TOP_MM + Light::RADIUS * 1.5
TRIGGER_Y_MM = TOP_MM + hp2mm(1.61)
INTERRUPT_Y_MM = TRIGGER_Y_MM + BUTTON_BUTTON_DISTANCE_MM
COMPLETION_Y_MM = INTERRUPT_Y_MM + BUTTON_BUTTON_DISTANCE_MM
START_ANCHOR_Y_MM = COMPLETION_Y_MM + ADVANCE_ANCHOR_DISTANCE_MM
END_ANCHOR_Y_MM = START_ANCHOR_Y_MM + ANCHOR_ANCHOR_DISTANCE_MM
DURATION_Y_MM = END_ANCHOR_Y_MM + ANCHOR_DURATION_DISTANCE
SHAPE_Y_MM = DURATION_Y_MM + DURATION_SHAPE_DISTANCE
ENABLED_Y_MM = BOTTOM_MM - Button::DIAMETER / 2.0

def position_marker(x:, y:, type:)
  marker = PositionMarker.new(type: type, color: @foreground)
  @controls << marker
  @image_shapes << marker.translated(x, y)
end

STEPPER_WIDTH_MM = STEP_DX_MM * 0.75
ANCHOR_SOURCES = %w[LEVEL IN OUT AUX]
ANCHOR_MODES = %w[SAMPLE TRACK]
STEP_LABEL_Y_MM = TOP_MM - hp2mm(0.25)

LINE_X_OFFSET_MM = -STEP_DX_MM / 2.0

TRIGGER_MODES = %w[RISE FALL EDGE HIGH LOW]
TRIGGER_MODE_SELECTION = 1
INTERRUPT_MODES = %w[IGNORE NEXT]
INTERRUPT_MODE_SELECTION = 1
COMPLETION_MODES = %w[SUSTAIN NEXT]
COMPLETION_MODE_SELECTION = 1
SHAPE_OPTIONS = %w[J S]
SHAPE_SELECTION = 1

def step_block(left)
  (0...STEPS).each do |step|
    x = left + step * STEP_DX_MM
    if step != 0
      # Skip the line for the first step.
      line_x = x + LINE_X_OFFSET_MM
      line x1: line_x, x2: line_x, y1: STEP_TOP_MM, y2: BOTTOM_MM
    end

    light x: x, y: PROGRESS_LIGHT_Y_MM
    label x: x, y: STEP_LABEL_Y_MM, text: (step + 1).to_s, alignment: :above, size: :large

    stepper x: x, y: TRIGGER_Y_MM, name: 'trigger-mode', options: TRIGGER_MODES, selection: TRIGGER_MODE_SELECTION, width: STEPPER_WIDTH_MM
    stepper x: x, y: INTERRUPT_Y_MM, name: 'interrupt-mode', options: INTERRUPT_MODES, selection: INTERRUPT_MODE_SELECTION, width: STEPPER_WIDTH_MM
    stepper x: x, y: COMPLETION_Y_MM, name: 'completion-mode', options: COMPLETION_MODES, selection: COMPLETION_MODE_SELECTION, width: STEPPER_WIDTH_MM

    stepper x: x, y: START_ANCHOR_Y_MM - KNOB_BUTTON_DISTANCE_MM, name: 'anchor-mode', options: ANCHOR_MODES, selection: 1, width: STEPPER_WIDTH_MM
    small_knob x: x, y: START_ANCHOR_Y_MM, label: ''
    stepper x: x, y: START_ANCHOR_Y_MM + KNOB_BUTTON_DISTANCE_MM, name: 'anchor-source', options: ANCHOR_SOURCES, selection: 3, width: STEPPER_WIDTH_MM

    stepper x: x, y: END_ANCHOR_Y_MM - KNOB_BUTTON_DISTANCE_MM, name: 'anchor-mode', options: ANCHOR_MODES, selection: 2, width: STEPPER_WIDTH_MM
    small_knob x: x, y: END_ANCHOR_Y_MM, label: ''
    stepper x: x, y: END_ANCHOR_Y_MM + KNOB_BUTTON_DISTANCE_MM, name: 'anchor-source', options: ANCHOR_SOURCES, selection: 1, width: STEPPER_WIDTH_MM

    stepper x: x, y: SHAPE_Y_MM - KNOB_BUTTON_DISTANCE_MM, name: 'shape', options: SHAPE_OPTIONS, selection: SHAPE_SELECTION, width: STEPPER_WIDTH_MM
    small_knob x: x, y: SHAPE_Y_MM, label: ''

    small_knob y: DURATION_Y_MM, x: x, label: ''

    button y: ENABLED_Y_MM, x: x, label: ''
  end

  start_position_marker_mm = left - Light::DIAMETER
  end_position_marker_mm = left + STEP_DX_MM * (STEPS - 1) + Light::DIAMETER

  position_marker(x: start_position_marker_mm, y: PROGRESS_LIGHT_Y_MM, type: :start)
  position_marker(x: end_position_marker_mm, y: PROGRESS_LIGHT_Y_MM, type: :end)
end
