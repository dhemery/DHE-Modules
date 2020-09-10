require_relative 'cv-sequencer/step-block'

margin = hp2mm(2.0)
width_hp = margin + 9.0 + STEP_BLOCK_WIDTH + margin

name "CV SEQUENCER"

hp width_hp

hue = 30
foreground [hue, 100, 10]
background [hue, 10, 93]

left = margin
right = hp2mm(width_hp) - margin
top = hp2mm(3.5)
bottom = hp2mm(23)


###############################################################################
#
# Sequence Controls (Left)
#
###############################################################################

sequence_controls_top = top + hp2mm(2.78)
sequence_controls_bottom = bottom - Port::DIAMETER / 2.0 - 1.0
sequence_controls_dy = (sequence_controls_bottom - sequence_controls_top) / 4
run_y = sequence_controls_top + 0 * sequence_controls_dy
loop_y = sequence_controls_top + 1 * sequence_controls_dy
selection_y = sequence_controls_top + 2 * sequence_controls_dy
gate_y = sequence_controls_top + 3 * sequence_controls_dy
reset_y = sequence_controls_top + 4 * sequence_controls_dy

input_button_port x: left, y: run_y, label: 'RUN'

input_button_port x: left, y: gate_y, label: 'GATE'

selection_length_x = left + hp2mm(1.63)
connector left: left, right: selection_length_x, y: selection_y
small_knob x: left - hp2mm(0.2), y: selection_y, label: 'START'
small_knob x: selection_length_x, y: selection_y, label: 'LENGTH'

input_button_port x: left, y: loop_y, label: 'LOOP'

input_button_port x: left, y: reset_y, label: 'RESET'


###############################################################################
#
# Step Controls
#
###############################################################################

step_x = hp2mm(8.5)

label_x = step_x - 0.6 * STEP_DX
label x: label_x, y: trigger_y(top), text: 'TRIG', alignment: :left_of, size: :large
label x: label_x, y: interrupt_y(top), text: 'INT', alignment: :left_of, size: :large
label x: label_x, y: completion_y(top), text: 'AT END', alignment: :left_of, size: :large
label x: label_x, y: start_anchor_y(top), text: 'START', alignment: :left_of, size: :large
label x: label_x, y: end_anchor_y(top), text: 'END', alignment: :left_of, size: :large
label x: label_x, y: shape_y(top) - hp2mm(0.25), text: 'SHAPE', alignment: :left_of, size: :large
label x: label_x, y: duration_y(top), text: 'DUR', alignment: :left_of, size: :large
label x: label_x, y: enabled_button_y(top), text: 'ON', alignment: :left_of, size: :large

step_block(top: top, bottom: bottom, left: left, foreground: @foreground, background: @background)


###############################################################################
#
# Sequence Controls (Right)
#
###############################################################################

near_right = right - hp2mm(2.2)

out_y = bottom - Port::DIAMETER / 2.0 - 1.0
in_y = sequence_controls_top

input_port x: near_right, y: in_y, label: 'IN'
input_port x: right, y: in_y, label: 'AUX'

polarity_y = (start_anchor_y(top) + end_anchor_y(top)) / 2.0
polarity_spread = (end_anchor_y(top) - start_anchor_y(top)) / 2.4
line x1: left + STEP_BLOCK_WIDTH, x2: near_right, y1: polarity_y - polarity_spread, y2: polarity_y
line x1: left + STEP_BLOCK_WIDTH, x2: near_right, y1: polarity_y + polarity_spread, y2: polarity_y
polarity_toggle x: near_right, y: polarity_y, selection: 2

connector left: left + STEP_BLOCK_WIDTH, right: near_right, y: duration_y(top)
duration_toggle x: near_right, y: duration_y(top)
port x: right, y: duration_y(top), label: 'CV'

output_port x: right, y: out_y, label: 'OUT'
