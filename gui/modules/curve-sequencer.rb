name 'CURVE  SEQUENCER'
hp 26

foreground [40, 0, 1]
background [220, 100, 93]

stages = 8

left = hp2mm(1.5)
top = hp2mm(5)

dx = hp2mm(3)
dy = hp2mm(3.4)

module_port_x = left + 0 * dx
module_control_x = left + 0.7 * dx

steps_y = top + 0 * dy
start_y = top + 1 * dy
loop_y = top + 2 * dy
reset_y = top + 3 * dy
gate_y = top + 4 * dy
out_y = top + 5 * dy

connector left: module_port_x, right: module_control_x, y: steps_y
port x: module_port_x, y: steps_y
small_knob x: module_control_x, y: steps_y, label: 'STEPS'

connector left: module_port_x, right: module_control_x, y: start_y
port x: module_port_x, y: start_y
small_knob x: module_control_x, y: start_y, label: 'START'

connector left: module_port_x, right: module_control_x, y: loop_y
port x: module_port_x, y: loop_y, label: 'LOOP'
button x: module_control_x, y: loop_y

connector left: module_port_x, right: module_control_x, y: reset_y
port x: module_port_x, y: reset_y, label: 'RESET'
button x: module_control_x, y: reset_y

connector left: module_port_x, right: module_control_x, y: gate_y
port x: module_port_x, y: gate_y, label: 'GATE'
button x: module_control_x, y: gate_y

output_port x: module_port_x, y: out_y, label: 'OUT'
output_port x: module_port_x + dy, y: out_y, label: 'EOC'

# polarity_toggle x: module_control_x, y: eoc_y
# duration_toggle x: module_control_x, y: out_y

left += hp2mm(7.5)
dx = hp2mm(2.5)
dy = hp2mm(2.1)

enabled_port_x = left + 0 * dx
enabled_button_x = left + 0.7 * dx
level_x = left + 1.7 * dx
duration_x = left + 2.7 * dx
curve_x = left + 3.7 * dx
shape_x = left + 4.4 * dx
mode_x = left + 5.2 * dx
eos_x = left + 6.2 * dx

(0..stages).each do |stage|
  y = top + stage * dy
  connector left: enabled_port_x, right: enabled_button_x, y: y
  port x: enabled_port_x, y: y, label: stage == 0 ? 'ON ' : ''
  button x: enabled_button_x, y: y, label: ''
  small_knob x: level_x, y: y, label: stage == 0 ? 'LEVEL' : ''
  connector left: curve_x, right: shape_x, y: y
  small_knob x: curve_x, y: y, label: stage == 0 ? 'CURVE' : ''
  stepper x: shape_x, y: y, name: 'shape', labels: %w[S J], selection: 1
  small_knob x: duration_x, y: y, label: stage == 0 ? 'DUR' : ' '
  stepper x: mode_x, y: y, name: 'mode', labels: %w[RISE FALL EDGE EOS LOW HIGH], selection: 1
  output_port x: eos_x, y: y, label: stage == 0 ? 'EOS' : ' '
end