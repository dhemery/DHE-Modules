name 'CURVE  SEQUENCER'
hp 34

foreground [40, 0, 1]
background [220, 100, 93]

stages = 8

left = hp2mm(2)

stage_label_y = hp2mm(3)
enabled_button_y = hp2mm(4)
enabled_port_y = enabled_button_y + hp2mm(1.6)
enabled_label_y = (enabled_button_y + enabled_port_y) / 2
mode_y = hp2mm(8.7)
level_y = hp2mm(11.2)
shape_y = hp2mm(14)
curve_y = shape_y + hp2mm(1.6)
duration_y = hp2mm(18.5)
eos_y = hp2mm(22)

top = enabled_label_y
bottom = eos_y
line_top = hp2mm(3.5)
line_bottom = hp2mm(23)

stage_x = hp2mm(10)
stage_dx = hp2mm(2.75)

label_x = stage_x - 0.6 * stage_dx
label x: label_x, y: enabled_label_y, text: 'ON', alignment: :left_of, size: :large
label x: label_x, y: mode_y, text: 'MODE', alignment: :left_of, size: :large
label x: label_x, y: (shape_y + curve_y) / 2, text: 'CURVE', alignment: :left_of, size: :large
label x: label_x, y: level_y, text: 'LEVEL', alignment: :left_of, size: :large
label x: label_x, y: duration_y, text: 'DUR ', alignment: :left_of, size: :large
label x: label_x, y: eos_y, text: 'EOS', alignment: :left_of, size: :large

(0...stages).each do |stage|
  x = stage_x + stage * stage_dx
  line_x = x - stage_dx / 2.0
  line x1: line_x, x2: line_x, y1: line_top, y2: line_bottom
  label x: x, y: stage_label_y, text: (stage + 1).to_s, alignment: :above, size: :large
  button y: enabled_button_y, x: x, label: ''
  port y: enabled_port_y, x: x, label: ''
  stepper x: x, y: shape_y, name: 'shape', labels: %w[J S]
  small_knob y: curve_y, x: x, label: ''
  small_knob y: level_y, x: x, label: ''
  small_knob y: duration_y, x: x, label: ''
  stepper y: mode_y, x: x, name: 'mode', labels: %w[RISE FALL EDGE EOS LOW HIGH], selection: 1
  output_port y: eos_y, x: x, label: ''
end

module_inputs_x = left
module_params_x = left + hp2mm(2)

input_dy = (bottom - top) / 5

run_y = top + 0 * input_dy
gate_y = top + 1 * input_dy
reset_y = top + 2 * input_dy
loop_y = top + 3 * input_dy
start_y = top + 4 * input_dy
steps_y = top + 5 * input_dy

connector left: module_inputs_x, right: module_params_x, y: steps_y
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

toggle_x = stage_x + stages * stage_dx
line_x = toggle_x - stage_dx / 2.0
line x1: line_x, x2: line_x, y1: line_top, y2: line_bottom
polarity_toggle x: toggle_x, y: level_y, selection: 2
duration_toggle x: toggle_x, y: duration_y
output_port x: toggle_x, y: eos_y, label: 'EOC'
output_port x: toggle_x, y: enabled_port_y, label: 'OUT'
