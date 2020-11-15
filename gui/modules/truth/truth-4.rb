require_relative 'helpers/make-truth'

self.class::include Truth

make_background 4

input_top = i2mm(1.1)
port_dy = i2mm(0.6)
input_x = i2mm(0.38)
output_x = input_x
output_top = input_top + 4.0 * port_dy

input_bounds = make_inputs n: 4, top: input_top, x: input_x, dy: port_dy
output_bounds = make_outputs top: output_top, x: output_x, dy: port_dy

table_x = input_bounds[:right] + (@width - input_bounds[:right]) / 2
table_top = input_bounds[:top]
table_bottom = output_bounds[:bottom]

make_table n: 4, top: table_top, bottom: table_bottom, x: table_x
