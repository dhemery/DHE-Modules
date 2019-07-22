name 'CURVE SEQUENCER 16'

steps = 16
width_hp = 49

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
# Sequence Controls
#
###############################################################################

input_top = top + 7.0
input_bottom = bottom - 4.2
input_dy = (input_bottom - input_top) / 5
run_y = input_top + 0 * input_dy
gate_y = input_top + 1 * input_dy
reset_y = input_top + 2 * input_dy
loop_y = input_top + 3 * input_dy
start_y = input_top + 4 * input_dy
steps_y = input_top + 5 * input_dy

module_inputs_x = left
module_params_x = left + hp2mm(2)

connector left: module_inputs_x, right: module_params_x, y: run_y
port x: module_inputs_x, y: run_y, label: 'RUN'
button x: module_params_x, y: run_y

connector left: module_inputs_x, right: module_params_x, y: loop_y
port x: module_inputs_x, y: loop_y, label: 'LOOP'
button x: module_params_x, y: loop_y

connector left: module_inputs_x, right: module_params_x, y: reset_y
port x: module_inputs_x, y: reset_y, label: 'RESET'
button x: module_params_x, y: reset_y

connector left: module_inputs_x, right: module_params_x, y: gate_y
port x: module_inputs_x, y: gate_y, label: 'GATE'
button x: module_params_x, y: gate_y

connector left: module_inputs_x, right: module_params_x, y: start_y
port x: module_inputs_x, y: start_y, label: 'START'
small_knob x: module_params_x, y: start_y

connector left: module_inputs_x, right: module_params_x, y: steps_y
port x: module_inputs_x, y: steps_y, label: 'STEPS'
small_knob x: module_params_x, y: steps_y




###############################################################################
#
# Step Controls
#
###############################################################################

step_x = hp2mm(10)
step_dx = hp2mm(2.25)

active_y = top + Light::RADIUS
mode_y = top + hp2mm(2.5)
level_y = top + hp2mm(5.25)
shape_y = top + hp2mm(8.5)
curve_y = top + hp2mm(10.75)
duration_y = top + hp2mm(13.75)
enabled_port_y = bottom - Port::DIAMETER / 2.0
enabled_button_y = enabled_port_y - Port::DIAMETER / 2.0 - Button::DIAMETER / 2.0 - 1.0

label_x = step_x - 0.6 * step_dx
label x: label_x, y: active_y, text: 'ACTIVE', alignment: :left_of, size: :large
label x: label_x, y: mode_y, text: 'MODE', alignment: :left_of, size: :large
label x: label_x, y: level_y, text: 'LEVEL', alignment: :left_of, size: :large
label x: label_x, y: shape_y, text: 'SHAPE', alignment: :left_of, size: :large
label x: label_x, y: curve_y, text: 'CURVE', alignment: :left_of, size: :large
label x: label_x, y: duration_y, text: 'DURATION', alignment: :left_of, size: :large
label x: label_x, y: enabled_button_y, text: 'ENABLED', alignment: :left_of, size: :large

line_x = step_x - step_dx / 2.0
line x1: line_x, x2: line_x, y1: top, y2: bottom

step_label_y = top - hp2mm(0.5)
(0...steps).each do |step|
  x = step_x + step * step_dx
  label x: x, y: step_label_y, text: (step + 1).to_s, alignment: :above, size: :large
  button y: enabled_button_y, x: x, label: ''
  port y: enabled_port_y, x: x, label: ''
  shape_toggle x: x, y: shape_y
  small_knob y: curve_y, x: x, label: ''
  small_knob y: level_y, x: x, label: ''
  small_knob y: duration_y, x: x, label: ''
  stepper y: mode_y, x: x, name: 'mode', labels: %w[RISE FALL EDGE EOC LOW HIGH], selection: 4
  light x: x, y: active_y
  line_x = x + step_dx / 2.0
  line x1: line_x, x2: line_x, y1: top, y2: bottom
end




###############################################################################
#
# Output Controls
#
###############################################################################

output_x = right
out_y = bottom - Port::DIAMETER / 2.0 - 1.0
polarity_toggle x: output_x, y: level_y, selection: 2
duration_toggle x: output_x, y: duration_y
output_port x: output_x, y: out_y, label: 'OUT'
